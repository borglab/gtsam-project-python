if(GTSAM_EXAMPLE_BUILD_PYTHON)
  set(PYBIND_WRAP_MODULE_NAME gtsam_example)
  
  # Generate setup.py.
  file(READ "${PROJECT_SOURCE_DIR}/README.md" README_CONTENTS)
  configure_file(${PROJECT_SOURCE_DIR}/python/setup.py.in
                 ${PROJECT_BINARY_DIR}/setup.py)

  add_subdirectory(pybind11)
  include(GtsamPybindWrap)

  add_custom_target(${PYBIND_WRAP_MODULE_NAME}_header DEPENDS "${PROJECT_SOURCE_DIR}/${PYBIND_WRAP_MODULE_NAME}.h")

  # ignoring the non-concrete types (type aliases)
  set(ignore
      )

  pybind_wrap(${PYBIND_WRAP_MODULE_NAME}_py # target
            ${PROJECT_SOURCE_DIR}/${PYBIND_WRAP_MODULE_NAME}.h # interface_header
            "${PYBIND_WRAP_MODULE_NAME}.cpp" # generated_cpp
            "${PYBIND_WRAP_MODULE_NAME}" # module_name
            "${PYBIND_WRAP_MODULE_NAME}" # top_namespace
            "${ignore}" # ignore_classes
            ${PROJECT_SOURCE_DIR}/wrap/pybind_wrapper.tpl
            ${PYBIND_WRAP_MODULE_NAME} # libs
            "${PYBIND_WRAP_MODULE_NAME};${PYBIND_WRAP_MODULE_NAME}_header" # dependencies
            )

  set_target_properties(${PYBIND_WRAP_MODULE_NAME}_py PROPERTIES
        INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/lib"
        INSTALL_RPATH_USE_LINK_PATH TRUE
        OUTPUT_NAME "${PYBIND_WRAP_MODULE_NAME}"
        LIBRARY_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/${PYBIND_WRAP_MODULE_NAME}"
        DEBUG_POSTFIX "" # Otherwise you will have a wrong name
        RELWITHDEBINFO_POSTFIX "" # Otherwise you will have a wrong name
        )

  # Symlink all tests .py files to build folder.
  create_symlinks("${CMAKE_CURRENT_SOURCE_DIR}/${PYBIND_WRAP_MODULE_NAME}_py/"
                  "${PROJECT_BINARY_DIR}/${PYBIND_WRAP_MODULE_NAME}")

  # Install.
  install(TARGETS ${PYBIND_WRAP_MODULE_NAME}_py DESTINATION "${GTSAM_EXAMPLE_PY_INSTALL_PATH}/${PYBIND_WRAP_MODULE_NAME}/")
  message("PYTHON INSTALL PATH: ${GTSAM_EXAMPLE_PY_INSTALL_PATH}/${PYBIND_WRAP_MODULE_NAME}")
  install_python_files("${PROJECT_BINARY_DIR}/setup.py"
                       "${GTSAM_EXAMPLE_PY_INSTALL_PATH}")
  install_python_scripts("${PROJECT_SOURCE_DIR}/python/${PYBIND_WRAP_MODULE_NAME}_py/"
                         "${GTSAM_EXAMPLE_PY_INSTALL_PATH}/${PYBIND_WRAP_MODULE_NAME}" "*.py")

endif()
