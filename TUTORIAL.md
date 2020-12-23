# Tutorial

This is a tutorial on how to wrap your own C++ projects using GTSAM's Pybind11 wrapper.

# Prerequisites

We assume you have Python 3 installed. We support Python 3.6 and up.

We also assume some knowledge of how Python packaging and setup works. If you understand how to write your own basic `setup.py` file, you should be fine.
Using this template project, you should only need to update the metadata information about your project. 

Check out the [python packaging website](https://packaging.python.org/tutorials/packaging-projects/) to learn more.

As a bonus, if you understand Pybind11's build process, this tutorial should be fairly intuitive.

**NOTE** This tutorial has been tested using GTSAM version 4.1.0 and above.

# Project Setup

As a set of minimal requirements, the project should be set up as follows:

```
top-level-directory
|
|- python/
  |- tests/
  |- __init__.py.in
  |- preamble.h
  |- requirements.txt
  |- setup.py.in
  |- specializations.h
|- src/
|- wrap
|- CMakeLists.txt
|- <project>.h
```

The files are

1. `python`: Directory with Python related files and meta-files.
    - `tests`: Directory of tests for the python module.
    - `__init__.py.in`: Template file for `__init__.py`.
    - `preamble.h`:
    - `requirements.txt`: Set of requirements needed by the wrapped module.
    - `setup.py.in`: Template file for `setup.py`.
    - `specializations.h`:
2. `src/`: All your C++ source code goes here.
3. `CMakeLists.txt`: The cmake definition file.
4. `<project>.h`: The header file which specifies all the code components to be wrapped.

You can add the `wrap` repository by running the file `update_wrap.sh`.

# CMake Configuration

In this section, we will go through a step-by-step process of defining the `CMakeLists.txt` file which will generated our wrapped code.

An illustrative example is provided in the `src` directory of this repository.

1. Define project name.
2. Optionally, set the Python version you'd like to target. This should ideally be the same as the version you used to build GTSAM and the GTSAM wrapper.
3. Include `GTSAM` package. This allows use to use the cython install path automatically. CMake will take care of the rest.

    ```cmake
    find_package(GTSAM REQUIRED)
    ```

4. These next few steps should be familiar for CMake users. We first include the project source directory.

    ```cmake
    include_directories(BEFORE "${PROJECT_SOURCE_DIR}")
    ```

5. Now we can specify the building and linking of our project code as a shared library.

    ```cmake
    add_library(${PROJECT_NAME} SHARED
                  src/greeting.h
                  src/greeting.cpp)
    target_link_libraries(${PROJECT_NAME} gtsam)
    ```

6. And finally, we can install the shared library.

    ```cmake
    install(TARGETS ${PROJECT_NAME} LIBRARY DESTINATION lib ARCHIVE DESTINATION lib RUNTIME DESTINATION bin)
    ```

7. Now we get to the wrapping part. We simply need to find the `gtwrap` package which contains the code for wrapping, and this will automatically include the CMake code for the Pybind wrapper.

    ```cmake
    find_package(gtwrap REQUIRED)
    ```

8. Next, to specify our project as a package, we need to include some files and metafiles, such as an `__init__.py` file at the top level and a `setup.py` file to install the wrapped code correctly. We also need a Pybind11 template file so that the wrapper can generate the C++ file that will be used by Pybind11 to generate the wrapped .so file.

    We can use the basic `__init__.py.in`, `setup.py.in`, and `pybind_wrapper.tpl.example` templates in this repo for convenience. Please adjust them as you see fit.

    - We set the version string to be set in the `setup.py`:
        ```cmake
        set(GTSAM_VERSION_STRING 0.0.1)
        ```
    
    - Now, we can include the various module and build files:
        ```cmake
        # We use this as a convenience variable.
        set(GTSAM_MODULE_PATH ${PROJECT_BINARY_DIR}/${PROJECT_NAME})

        # Add the setup.py file
        configure_file(${PROJECT_SOURCE_DIR}/python/setup.py.in
                       ${GTSAM_MODULE_PATH}/setup.py)
        
        # Add the __init__.py file
        configure_file(${PROJECT_SOURCE_DIR}/python/__init__.py.in
                       ${GTSAM_MODULE_PATH}/${PROJECT_NAME}/__init__.py)

        # Add the Pybind11 template file.
        configure_file(${PROJECT_SOURCE_DIR}/wrap/pybind_wrapper.tpl.example
                       ${PROJECT_BINARY_DIR}/${PROJECT_NAME}.tpl)
        
        ```

    - Finally, we copy over the tests. More such copy operations can be added for other types of files.

        ```cmake
        # Copy all the tests files to module folder.
        file(COPY "${PROJECT_SOURCE_DIR}/python/tests"
             DESTINATION "${GTSAM_MODULE_PATH}")
        ```

9. We now specify the wrapping function so that the GTSAM wrapper can do its job. We require only one function `pybind_wrap` which takes the following 9 arguments:

    1. Target: The name of the make target with which the wrapping process is associated.
    2. Interface Header: A `.h` (or `.i`) file which defines what classes, functions, etc., are to be wrapped.
    3. Generated Cpp: The name of the generated .cpp file which Pybind11 will use.
    4. Module Name: The name of the Python module. Required for Pybdind11.
    5. Top Namespace: The top-level namespace in the C++ source code.
    6. Classes To Ignore: The list of classes to ignore from wrapping. This is generally applied for classes which are type aliases (aka `typedef`).
    7. Pybind11 Template: The template file which will be used to generated the .cpp Pybind11 file.
    8. Library Name: The name of the library .so file that will be generated on compilation.
    9. CMake Dependencies: A comma-separated list of CMake dependencies that need to be built before the code is to be wrapped. At the very least, this should be the library we defined in step 6.

    ```cmake
    pybind_wrap(${PROJECT_NAME}_py # target
            ${PROJECT_SOURCE_DIR}/${PROJECT_NAME}.h # interface header file (gtsam_example.h in the root directory)
            "${PROJECT_NAME}.cpp" # the generated cpp (gtsam_example.cpp)
            "${PROJECT_NAME}" # module_name (gtsam_example)
            "${PROJECT_NAME}" # top namespace in the cpp file (gtsam_example)
            "" # ignore classes
            ${PROJECT_BINARY_DIR}/${PROJECT_NAME}.tpl
            ${PROJECT_NAME} # libs
            "${PROJECT_NAME}" # dependencies, we need the library built in step 6 as the minimum.
          )
    ```

    To save the generated .so library in the correct place and ensure correct naming, we set the appropriate properties on the Make target:

        ```cmake
        set_target_properties(${PROJECT_NAME}_py PROPERTIES
                OUTPUT_NAME "${PROJECT_NAME}"
                LIBRARY_OUTPUT_DIRECTORY "${GTSAM_MODULE_PATH}/${PROJECT_NAME}"
                DEBUG_POSTFIX "" # Otherwise you will have a wrong name
                RELWITHDEBINFO_POSTFIX "" # Otherwise you will have a wrong name
        )
        ```

10. Finally, we add a custom `make` command to make (pun unintended) installing the generated python package super easy.

    ```cmake
    add_custom_target(python-install
        COMMAND ${PYTHON_EXECUTABLE} ${GTSAM_MODULE_PATH}/setup.py install
        DEPENDS gtsam_example_py
        WORKING_DIRECTORY ${GTSAM_MODULE_PATH})
    ```

# Compiling

To compile and wrap the code, the familiar CMake process is followed. Starting from the top-level root directory where the `gtsam_example.h` file is located, we create a build directory and run `cmake` and `make`.

```sh
mkdir build && cd build
cmake .. && make
```

# Installing

To install the package, in the `build` directory we can run `make python-install`.
