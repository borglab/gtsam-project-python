# This file should be used as a template for creating new projects with Python wrapping using the CMake tools

###################################################################################
# 1. To create your own project, replace "gtsam_example" with the actual name of your project
cmake_minimum_required(VERSION 3.0)
project(gtsam_example CXX C)

###################################################################################
# 2. Set the python version
set(GTSAM_PYTHON_VERSION "Default" CACHE STRING "The Python version to use for wrapping")
if(GTSAM_PYTHON_VERSION STREQUAL "Default")
  find_package(PythonInterp REQUIRED)
  find_package(PythonLibs REQUIRED)
else()
  find_package(PythonInterp ${GTSAM_PYTHON_VERSION} EXACT REQUIRED)
  find_package(PythonLibs ${GTSAM_PYTHON_VERSION} EXACT REQUIRED)
endif()

###################################################################################
# 3. Find GTSAM components so we have access to the GTSAM Cython install path
find_package(GTSAM REQUIRED) # Uses installed package
# Note: Since Jan-2019, GTSAMConfig.cmake defines exported CMake targets
# that automatically do include the include_directories() without the need
# to call include_directories(), just target_link_libraries(NAME gtsam)
#include_directories(${GTSAM_INCLUDE_DIR})

# Include the required GTSAM Cython libraries
include_directories(${GTSAM_CYTHON_INSTALL_PATH})
include_directories(${GTSAM_EIGENCY_INSTALL_PATH})

###################################################################################
# 4. Get the wrapping functions
# Include GTSAM CMake tools
find_package(GTSAMCMakeTools CONFIG)
#include(GtsamBuildTypes) # Load build type flags and default to Debug mode
#include(GtsamTesting)    # Easy functions for creating unit tests and scripts
include(GtsamCythonWrap) # Automatic Cython wrapper generation

###################################################################################
# 5. Add the local source directory for CMake
# Ensure that local folder is searched before library folders
include_directories(BEFORE "${PROJECT_SOURCE_DIR}")


###################################################################################
# 6. Build static library from common sources
add_library(${PROJECT_NAME} SHARED src/greeting.h src/greeting.cpp)
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
# 8. Copy the __init__.py file so Cython recognizes this as a package.
# This function also updates the contents to use the correct package name.
configure_file(${PROJECT_SOURCE_DIR}/__init__.py.in ${PROJECT_BINARY_DIR}/cython/${PROJECT_NAME}/__init__.py)

###################################################################################
# 9. (Strict) Copy over the setup.py file so we can build and install the package.
configure_file(${PROJECT_SOURCE_DIR}/setup.py ${PROJECT_BINARY_DIR}/cython/setup.py COPYONLY)

###################################################################################
# 10. Build Cython wrapper (CMake tracks the dependecy to link with GTSAM through our project's static library)
wrap_and_install_library_cython("gtsam_example.h" # interface_header
                                "" # extra imports
                                "${PROJECT_BINARY_DIR}/cython/${PROJECT_NAME}" # install path
                                "gtsam;${PROJECT_NAME}"  # library to link with
                                "wrap;gtsam"  # dependencies which need to be built before wrapping
                                )
