# Tutorial

This is a tutorial on how to wrap your own C++ projects using GTSAM's python wrapper.

# Prerequisites

We assume you have Python 3 installed. We support Python 3.6 and up.

We also assume some knowledge of how Python packaging and setup works. If you understand how to write your own basic `setup.py` file, you should be fine.
Using this template project, you should only need to update the metadata information about your project. Check out the [python packaging website](https://packaging.python.org/tutorials/packaging-projects/) to learn more.

As a bonus, if you understand Cython's build process, this tutorial should be fairly intuitive.

**NOTE** This tutorial has been tested using GTSAM version 4.0.x and above.

# Project Setup

As a set of minimal requirements, the project should be set up as follows:

```
top-level-directory
|
|- CMakeLists.txt
|- <project>.h
|- __init__.py.in
|- setup.py
|- src/

```

The files are

1. `CMakeLists.txt`: The cmake definition file.
2. `<project>.h`: The header file which specifies all the code components to be wrapped.
3. `__init__.py.in`: Template __init__.py file used by cmake.
4. `setup.py`: The file used by setuptools to generate the egg/wheel.
5. `src/`: All your C++ source code goes here.


# CMake Configuration

In this section, we will go through a step-by-step process of defining the `CMakeLists.txt` file which will generated our wrapped code.

An illustrative example is provided in the `src` directory of this repository.

1. Define project name.
2. Optionally, set the Python version you'd like to target. This should ideally be the same as the version you used to build the wrapper.
3. Include `GTSAM` package. This allows use to use the cython install path automatically. CMake will take care of the rest.

    ```cmake
    find_package(GTSAM REQUIRED)
    include_directories(${GTSAM_CYTHON_INSTALL_PATH})
    include_directories(${GTSAM_EIGENCY_INSTALL_PATH})
    ```

 4. The second package is `GTSAMCMakeTools`. This gives us access to the wrapping functions which we will use later on.

    ```cmake
    find_package(GTSAMCMakeTools CONFIG)
    include(GtsamCythonWrap) # Automatic Cython wrapper generation
    ```

5. These next few steps should be familiar for CMake users. We first include the project source directory.

    ```cmake
    include_directories(BEFORE "${PROJECT_SOURCE_DIR}")
    ```

6. Now we can specify the building and linking of our project code as a shared library.

    ```cmake
    add_library(${PROJECT_NAME} SHARED src/greeting.h src/greeting.cpp)
    target_link_libraries(${PROJECT_NAME} gtsam)
    ```

7. And finally, we can install the shared library.

    ```cmake
    install(TARGETS ${PROJECT_NAME} LIBRARY DESTINATION lib ARCHIVE DESTINATION lib RUNTIME DESTINATION bin)
    ```

8. Now we get to the wrapping part. To specify our project as a package we need to include an `__init__.py` file at the top level. This will allow python imports to work correctly. We can use the basic `__init__.py.in` template in this repo since it is pretty generic.

    ```cmake
    configure_file(${PROJECT_SOURCE_DIR}/__init__.py.in ${PROJECT_BINARY_DIR}/cython/${PROJECT_NAME}/__init__.py)
    ```

9. To help build and install the wrapped project, we make use of a `setup.py` file. This file can be customized as per your requirements. **NOTE** This command only copies over the `setup.py` file, so make sure you make any updates **BEFORE** you run `cmake`.

    ```cmake
    configure_file(${PROJECT_SOURCE_DIR}/setup.py ${PROJECT_BINARY_DIR}/cython/setup.py COPYONLY)
    ```

10. Finally, we specify the wrapping function so that the GTSAM wrapper can do its job. We require only one function `wrap_and_install_library_cython` which takes the following 5 arguments:

    1. Interface Header: A `.h` file which defines what classes, functions, etc., are to be wrapped.
    2. Extra Imports: This is a set of `cython` imports included in the generated Cython files. You can use this to specify any additional imports your project may be dependent on.
    3. Install Path: This is the location where the wrapped package will be installed on running `make install`.
    4. Libraries: A semi-colon separated list of libraries which the project will be linked against. At the very least, you should link against `gtsam` and the generated shared object file.
    5. Dependencies: This is a semi-colon separated list of dependency targets that need to be built before the code can be compiled and wrapped. This is nothing but a list of CMake targets.

    ```cmake
    wrap_and_install_library_cython("example.h" # interface_header
                                    "" # extra imports
                                    "./${PROJECT_NAME}" # install path
                                    "gtsam;${PROJECT_NAME}"  # library to link with
                                    "wrap;gtsam"  # dependencies which need to be built before wrapping
                                    )
    ```

# Compiling

To compile and wrap the code, the familiar CMake process is followed. Starting from the directory where the `setup.py` file is located, we create a build directory and run `cmake` and `make`.

```sh
mkdir build && cd build
cmake .. && make
```

Finally, we go into the generated `cython` directory where the `setup.py` file is present, and run `python setup.py build` to generate the final package.

# Installing

To install the package, in the `cython` directory we can run `python setup.py build`.
