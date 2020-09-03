# This file should be used as a template for creating new projects with Python wrapping using the CMake tools

###################################################################################
# 1. To create your own project, replace "gtsam_example" with the actual name of your project
cmake_minimum_required(VERSION 3.0)
project(gtsam_example CXX C)
set(CXX_STANDARD 11)

###################################################################################
# 2. Set the python version
# Load the default Python version strings
find_package (Python COMPONENTS Interpreter Development)

set(WRAP_PYTHON_VERSION "${Python_VERSION_MAJOR}.${PYTHON_VERSION_MINOR}" CACHE STRING "The Python version to use for wrapping")
if(WRAP_PYTHON_VERSION STREQUAL "${Python_VERSION_MAJOR}.${PYTHON_VERSION_MINOR}")
  find_package(PythonInterp REQUIRED)
  find_package(PythonLibs REQUIRED)
else()
  find_package(PythonInterp ${WRAP_PYTHON_VERSION} EXACT REQUIRED)
  find_package(PythonLibs ${WRAP_PYTHON_VERSION} EXACT REQUIRED)
endif()

###################################################################################
# 3. Find GTSAM components so we have access to the GTSAM Cython install path
find_package(GTSAM REQUIRED) # Uses installed package
# Note: Since Jan-2019, GTSAMConfig.cmake defines exported CMake targets
# that automatically do include the include_directories() without the need
# to call include_directories(), just target_link_libraries(NAME gtsam)
#include_directories(${GTSAM_INCLUDE_DIR})

###################################################################################
# 4. Get the wrapping functions
# Include GTSAM CMake tools
find_package(GTSAMCMakeTools CONFIG)
#include(GtsamBuildTypes) # Load build type flags and default to Debug mode
#include(GtsamTesting)    # Easy functions for creating unit tests and scripts

###################################################################################
# 5. Add the local source directory for CMake
# Ensure that local folder is searched before library folders
include_directories(BEFORE "${PROJECT_SOURCE_DIR}")

###################################################################################
# 6. Build static library from common sources.
# This is a common cmake step where we load all the source files
# and link against the GTSAM library.
add_library(${PROJECT_NAME} SHARED
              cpp/greeting.h
              cpp/greeting.cpp)
target_link_libraries(${PROJECT_NAME} gtsam)

###################################################################################
# 7. Install library
install(TARGETS ${PROJECT_NAME} LIBRARY DESTINATION lib ARCHIVE DESTINATION lib RUNTIME DESTINATION bin)

###################################################################################
# # Build tests (CMake tracks the dependecy to link with GTSAM through our project's static library)
# gtsamAddTestsGlob("${PROJECT_NAME}" "tests/test*.cpp" "" "${PROJECT_NAME}")

###################################################################################
# # Build scripts (CMake tracks the dependecy to link with GTSAM through our project's static library)
# gtsamAddExamplesGlob("*.cpp" "" "${PROJECT_NAME}")

###################################################################################
# 8. Add the path to the wrap/cmake directory and include the cmake code for wrapping.
# Please update this accordingly so it points to the wrap subtree and
# the cmake subdirectory in particular.
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/wrap/cmake")
include(PybindWrap)

###################################################################################
# 9. Configure all the files for the Python module.
# Set the version string
set(GTSAM_VERSION_STRING 0.0.1) # Needed for generating setup.py

# We use this as a convenience variable.
# It points to `python/` in this case.
set(GTSAM_MODULE_PATH ${PROJECT_SOURCE_DIR}/python)

# Copy over the setup.py file.
# This is now `python/setup.py`.
configure_file(${PROJECT_SOURCE_DIR}/setup.py.in
               ${GTSAM_MODULE_PATH}/setup.py)

# Copy over the __init__.py file.
# This is now `python/gtsam_example/__init__.py`.
configure_file(${PROJECT_SOURCE_DIR}/__init__.py.in
               ${GTSAM_MODULE_PATH}/${PROJECT_NAME}/__init__.py)

# Copy over the pybind .tpl file.
# This is now `build/gtsam_example.tpl`.
configure_file(${PROJECT_SOURCE_DIR}/${PROJECT_NAME}.tpl
               ${PROJECT_BINARY_DIR}/${PROJECT_NAME}.tpl)

# Copy all the tests files to module folder.
file(COPY "${PROJECT_SOURCE_DIR}/python/tests"
DESTINATION "${GTSAM_MODULE_PATH}")

# if (NOT GTSAM_TYPEDEF_POINTS_TO_VECTORS)
# 	message(FATAL_ERROR "GTSAM_EXAMPLE_BUILD_PYTHON requires GTSAM_TYPEDEF_POINTS_TO_VECTORS to be enabled but it is not.")
# endif()
# set(GTSAM_EXAMPLE_PY_INSTALL_PATH "${CMAKE_INSTALL_PREFIX}/python")

###################################################################################
# 10. Build Pybind wrapper
# This is where the crux of the wrapping happens.

# Ignoring the non-concrete types (type aliases and typedefs).
# We don't have any in our current project so this is left as empty.
set(ignore "")

# This is the main function that generates the cpp file which Pybind11 will use.
pybind_wrap(${PROJECT_NAME}_py # target
            ${PROJECT_SOURCE_DIR}/${PROJECT_NAME}.h # interface header file (gtsam_example.h in the root directory)
            "${PROJECT_NAME}.cpp" # the generated cpp (gtsam_example.cpp)
            "${PROJECT_NAME}" # module_name (gtsam_example)
            "${PROJECT_NAME}" # top namespace in the cpp file (gtsam_example)
            "${ignore}" # ignore classes
            ${PROJECT_BINARY_DIR}/${PROJECT_NAME}.tpl
            ${PROJECT_NAME} # libs
            "${PROJECT_NAME}" # dependencies, we need the library built in step 6 as the minimum.
          )

# We define where we wish to save the wrapped .so file once we run `make`.
# It will be stored in `build/gtsam_example/gtsam_example` to conform to standard python module structure.
set_target_properties(${PROJECT_NAME}_py PROPERTIES
        OUTPUT_NAME "${PROJECT_NAME}"
        LIBRARY_OUTPUT_DIRECTORY "${GTSAM_MODULE_PATH}/${PROJECT_NAME}"
        DEBUG_POSTFIX "" # Otherwise you will have a wrong name
        RELWITHDEBINFO_POSTFIX "" # Otherwise you will have a wrong name
        )

###################################################################################
# 11. Python install command
# Finally we add a custom make command which we can invoke to install the generated module.
# Simply type `make python-install` and we can now access the wrapped module as an installed library.
add_custom_target(python-install
        COMMAND ${PYTHON_EXECUTABLE} ${GTSAM_MODULE_PATH}/setup.py install
        DEPENDS gtsam_example_py
        WORKING_DIRECTORY ${GTSAM_MODULE_PATH})
