# This file should be used as a template for creating new projects with Python
# wrapping using the CMake tools

# ##############################################################################
# * 1. To create your own project, replace "gtsam_example" with the actual name
#   of your project
cmake_minimum_required(VERSION 3.9)
project(gtsam_example CXX C)
set(CXX_STANDARD 11)

# ##############################################################################
# * 2. Find GTSAM components so we have access to the GTSAM Cython install path
find_package(GTSAM REQUIRED) # Uses installed package
# Note: Since Jan-2019, GTSAMConfig.cmake defines exported CMake targets that
# automatically do include the include_directories() without the need to call
# include_directories(), just target_link_libraries(NAME gtsam)
# include_directories(${GTSAM_INCLUDE_DIR})

# ##############################################################################
# * 3. Add the local source directory for CMake Ensure that local folder is
#   searched before library folders
include_directories(BEFORE "${PROJECT_SOURCE_DIR}")

# ##############################################################################
# * 4. Build static library from common sources. This is a common cmake step
#   where we load all the source files and link against the GTSAM library.
add_library(${PROJECT_NAME} SHARED cpp/greeting.h cpp/greeting.cpp)
target_link_libraries(${PROJECT_NAME} gtsam)

# ##############################################################################
# * 5. Install library
install(
  TARGETS ${PROJECT_NAME}
  LIBRARY DESTINATION lib
  ARCHIVE DESTINATION lib
  RUNTIME DESTINATION bin)

# ##############################################################################
# # Build tests (CMake tracks the dependecy to link with GTSAM through our
# project's static library) gtsamAddTestsGlob("${PROJECT_NAME}"
# "tests/test*.cpp" "" "${PROJECT_NAME}")

# ##############################################################################
# # Build scripts (CMake tracks the dependecy to link with GTSAM through our
# project's static library) gtsamAddExamplesGlob("*.cpp" "" "${PROJECT_NAME}")

# ##############################################################################
# * 6. Find the gtwrap package (either installed via GTSAM or as a standalone
#   package) The cmake code for wrapping is included automatically.
find_package(gtwrap REQUIRED)

# ##############################################################################
# * 7. Set the python version Load the default Python version strings and
#   variables.
set(WRAP_PYTHON_VERSION
    "Default"
    CACHE STRING "The Python version to use for wrapping")

gtwrap_get_python_version(${WRAP_PYTHON_VERSION})

# ##############################################################################
# * 8. Configure all the files for the Python module. Set the version string,
#   needed for generating setup.py
set(GTSAM_VERSION_STRING 0.0.1)

# We use this as a convenience variable. It points to `python/` in this case.
set(GTSAM_MODULE_PATH ${PROJECT_BINARY_DIR}/python)

# Copy over the setup.py.in file. This will become `python/setup.py`.
configure_file(${PROJECT_SOURCE_DIR}/wrapper/setup.py.in
               ${GTSAM_MODULE_PATH}/setup.py)

# Copy over the __init__.py file. This is now
# `python/gtsam_example/__init__.py`.
configure_file(${PROJECT_SOURCE_DIR}/wrapper/__init__.py.in
               ${GTSAM_MODULE_PATH}/${PROJECT_NAME}/__init__.py)

# Copy over the pybind .tpl file. This is now `build/gtsam_example.tpl`.
configure_file(${PROJECT_SOURCE_DIR}/wrapper/pybind_wrapper.tpl.example
               ${PROJECT_BINARY_DIR}/${PROJECT_NAME}.tpl)

# Copy over C++ headers to use for wrapping
file(COPY "${PROJECT_SOURCE_DIR}/wrapper/preamble.h"
     DESTINATION "${PROJECT_BINARY_DIR}/python")

file(COPY "${PROJECT_SOURCE_DIR}/wrapper/specializations.h"
     DESTINATION "${PROJECT_BINARY_DIR}/python")

# Copy all the tests files to module folder.
file(COPY "${PROJECT_SOURCE_DIR}/python/tests"
     DESTINATION "${GTSAM_MODULE_PATH}")

# We print out our configuration for an easy visual check
message("========== Configuration Options ==========")
message(STATUS "Project:                  ${PROJECT_NAME}")
message(STATUS "Python Version:           ${WRAP_PYTHON_VERSION}")
message(STATUS "Python Module Path:       ${GTSAM_MODULE_PATH}")
message("===========================================")


# ##############################################################################
# * 9. Build Pybind wrapper This is where the crux of the wrapping happens.

# Ignoring the non-concrete types (type aliases and typedefs). We don't have any
# in our current project so this is left as empty.
set(ignore "")

# This is the main function that generates the cpp file which Pybind11 will use.
pybind_wrap(
  ${PROJECT_NAME}_py # target
  ${PROJECT_SOURCE_DIR}/${PROJECT_NAME}.i # interface header file
                                          # (gtsam_example.i in the root
                                          # directory)
  "${PROJECT_NAME}.cpp" # the generated cpp (gtsam_example.cpp)
  "${PROJECT_NAME}" # module_name (gtsam_example)
  "${PROJECT_NAME}" # top namespace in the cpp file (gtsam_example)
  "${ignore}" # ignore classes
  ${PROJECT_BINARY_DIR}/${PROJECT_NAME}.tpl
  ${PROJECT_NAME} # libs
  "${PROJECT_NAME}" # dependencies, we need the library built in step 6 as the
                    # minimum.
  ON # we are using boost::shared_ptr not std
)

# We define where we wish to save the wrapped .so file once we run `make`. It
# will be stored in `build/gtsam_example/gtsam_example` to conform to standard
# python module structure.
set_target_properties(
  ${PROJECT_NAME}_py
  PROPERTIES OUTPUT_NAME "${PROJECT_NAME}"
             LIBRARY_OUTPUT_DIRECTORY "${GTSAM_MODULE_PATH}/${PROJECT_NAME}"
             DEBUG_POSTFIX "" # Otherwise you will have a wrong name
             RELWITHDEBINFO_POSTFIX "" # Otherwise you will have a wrong name
)

# ##############################################################################
# * 10. Python install command Finally we add a custom make command which we can
#   invoke to install the generated module. Simply type `make python-install`
#   and we can now access the wrapped module as an installed library.
add_custom_target(
  python-install
  COMMAND ${PYTHON_EXECUTABLE} ${GTSAM_MODULE_PATH}/setup.py install
  DEPENDS ${PROJECT_NAME}_py
  WORKING_DIRECTORY ${GTSAM_MODULE_PATH})
