# This file should be used as a template for creating new projects with Python wrapping using the CMake tools

###################################################################################
# 1. To create your own project, replace "gtsam_example" with the actual name of your project
cmake_minimum_required(VERSION 3.0)
project(gtsam_example CXX C)
set(CXX_STANDARD 11)

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
# 10. Build Pybind wrapper
set(GTSAM_EXAMPLE_VERSION_STRING 0.0.1) # Needed for generating setup.py

if(GTSAM_EXAMPLE_BUILD_PYTHON)
  list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/wrap/cmake")
	# if (NOT GTSAM_TYPEDEF_POINTS_TO_VECTORS)
	# 	message(FATAL_ERROR "GTSAM_EXAMPLE_BUILD_PYTHON requires GTSAM_TYPEDEF_POINTS_TO_VECTORS to be enabled but it is not.")
	# endif()
	set(GTSAM_EXAMPLE_PY_INSTALL_PATH "${CMAKE_INSTALL_PREFIX}/python")
endif()
add_subdirectory(python)