# gtsam-project-python
Project template using GTSAM + python wrapping


INSTALL
=======
- Python3 is required
- Make the following changes before build and install:
    - Set the CMake flag `GTSAM_INSTALL_CYTHON_TOOLBOX=ON` to enable building the cython wrapper.
    - Set the CMake flag `-DGTSAM_PYTHON_VERSION=3`, otherwise the default interpreter will be used.
- Run `cmake ..`
- Run `make install` (may need use sudo), and the wrapped module will be installed to `GTSAM_CYTHON_INSTALL_PATH`, which is
by default: `<your CMAKE_INSTALL_PREFIX>/cython`
- After building and installing gtsam, navigate to `GTSAM_CYTHON_INSTALL_PATH` and run `python setup.py install`

For more detailed information, please see 
https://github.com/borglab/gtsam/tree/develop/cython