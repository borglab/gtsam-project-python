# gtsam-project-python
Project template using GTSAM + python wrapping


INSTALL
=======
- Python3 is required
- Make the following changes before build and install:
    - enable `GTSAM_INSTALL_CYTHON_TOOLBOX`
    - if you want to build the gtsam python library for a specific python version (eg 3.6), use the `-DGTSAM_PYTHON_VERSION=3.6` option, otherwise the default interpreter will be used.
- Run `cmake ..`
- Run `make install` (may need use sudo), and the wrapped module will be installed to `GTSAM_CYTHON_INSTALL_PATH`, which is
by default: `<your CMAKE_INSTALL_PREFIX>/cython`
- After build and install gtsam, navigate to `GTSAM_CYTHON_INSTALL_PATH` and run `python setup.py install`

For more detailed information, please see 
https://github.com/borglab/gtsam/tree/develop/cython