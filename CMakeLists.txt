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
add_library(${PROJECT_NAME} SHARED cpp/greeting.h cpp/greeting.cpp
                                   cpp/sub_dir/curse.h cpp/sub_dir/curse.cpp)
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
# Upto this point, we've only focused on building the C++ library. For details
# on how to build the python wrapper, please see the CMakeLists.txt file in the
# `python` directory.
add_subdirectory(python)
