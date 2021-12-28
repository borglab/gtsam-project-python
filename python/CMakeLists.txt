# ##############################################################################
# * 1. Find the gtwrap package (either installed via GTSAM or as a standalone
#   package) The cmake code for wrapping is included automatically.
find_package(gtwrap REQUIRED)

# ##############################################################################
# * 2. Set the python version Load the default Python version strings and
#   variables.
set(WRAP_PYTHON_VERSION
    "Default"
    CACHE STRING "The Python version to use for wrapping")

gtwrap_get_python_version(${WRAP_PYTHON_VERSION})

# ##############################################################################
# * 3. Configure all the files for the Python module. Set the version string,
#   needed for generating setup.py
set(GTSAM_VERSION_STRING 0.0.1)

# We use this as a convenience variable. It points to `python/` in this case.
set(PROJECT_PY_MODULE_PATH ${PROJECT_BINARY_DIR}/python)

# Copy over the setup.py.in file. This will become `python/setup.py`.
configure_file(${PROJECT_SOURCE_DIR}/wrapper/setup.py.in
               ${PROJECT_PY_MODULE_PATH}/setup.py)

# Copy over the __init__.py file. This is now
# `python/gtsam_example/__init__.py`.
configure_file(${PROJECT_SOURCE_DIR}/wrapper/__init__.py.in
               ${PROJECT_PY_MODULE_PATH}/${PROJECT_NAME}/__init__.py)

# Copy over the pybind .tpl file. This is now `build/gtsam_example.tpl`.
configure_file(${PROJECT_SOURCE_DIR}/wrapper/pybind_wrapper.tpl.example
               ${PROJECT_PY_MODULE_PATH}/${PROJECT_NAME}.tpl)

# Define the list of interface files.
set(interface_list ${PROJECT_SOURCE_DIR}/${PROJECT_NAME}.i
                   ${PROJECT_SOURCE_DIR}/cpp/sub_dir/sub_dir.i)

# Copy over C++ headers to use for wrapping
foreach(interface ${interface_list})
  get_filename_component(interface_name ${interface} NAME_WE)

  file(COPY "${PROJECT_SOURCE_DIR}/wrapper/preamble/${interface_name}.h"
       DESTINATION "${PROJECT_PY_MODULE_PATH}/${PROJECT_NAME}/preamble")

  file(COPY "${PROJECT_SOURCE_DIR}/wrapper/specializations/${interface_name}.h"
       DESTINATION "${PROJECT_PY_MODULE_PATH}/${PROJECT_NAME}/specializations")

endforeach()

# Copy all the tests files to module folder.
file(COPY "${PROJECT_SOURCE_DIR}/python/tests"
     DESTINATION "${PROJECT_PY_MODULE_PATH}")

# We print out our configuration for an easy visual check
message("========== Configuration Options ==========")
message(STATUS "Project:                  ${PROJECT_NAME}")
message(STATUS "Python Version:           ${WRAP_PYTHON_VERSION}")
message(STATUS "Python Module Path:       ${PROJECT_PY_MODULE_PATH}")
message("===========================================")

# ##############################################################################
# * 4. Build Pybind wrapper This is where the crux of the wrapping happens.

# Ignoring the non-concrete types (type aliases and typedefs). We don't have any
# in our current project so this is left as empty.
set(ignore "")

# This is the main function that generates the cpp file which Pybind11 will use.
pybind_wrap(
  ${PROJECT_NAME}_py # target
  "${interface_list}" # interface header files (gtsam_example.i in the root
                      # directory)
  "${PROJECT_NAME}.cpp" # the generated cpp (gtsam_example.cpp)
  "${PROJECT_NAME}" # module_name (gtsam_example)
  "${PROJECT_NAME}" # top namespace in the cpp file (gtsam_example)
  "${ignore}" # ignore classes
  ${PROJECT_PY_MODULE_PATH}/${PROJECT_NAME}.tpl
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
             LINKER_LANGUAGE CXX
             LIBRARY_OUTPUT_DIRECTORY
             "${PROJECT_PY_MODULE_PATH}/${PROJECT_NAME}"
             DEBUG_POSTFIX "" # Otherwise you will have a wrong name
             RELWITHDEBINFO_POSTFIX "" # Otherwise you will have a wrong name
)

# ##############################################################################
# * 5. Python install command. We add a custom make command which we can invoke
#   to install the generated module. Simply type `make python-install` and we
#   can now access the wrapped module as an installed library.
add_custom_target(
  python-install
  COMMAND ${PYTHON_EXECUTABLE} ${PROJECT_PY_MODULE_PATH}/setup.py install
  DEPENDS ${PROJECT_NAME}_py
  WORKING_DIRECTORY ${PROJECT_PY_MODULE_PATH})

# ##############################################################################
# * 6. Python test command. Simply type `make python-test` to run all the python
#   tests in this project.
add_custom_target(
  python-test
  COMMAND
    ${CMAKE_COMMAND} -E env # add package to python path so no need to install
    "PYTHONPATH=${PROJECT_BINARY_DIR}/$ENV{PYTHONPATH}" ${PYTHON_EXECUTABLE} -m
    unittest discover -v -s .
  DEPENDS ${PROJECT_NAME}_py
  WORKING_DIRECTORY "${PROJECT_BINARY_DIR}/tests")
