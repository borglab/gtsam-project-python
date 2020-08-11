# gtsam-project-python

Project template using GTSAM + python wrapping

## PREREQUISITES

- Python 3.6+ is required, since we support these versions.
- Make the following changes to `GTSAM`:

  - Set the CMake flag `GTSAM_INSTALL_CYTHON_TOOLBOX` to `ON` to enable building the cython wrapper.
  - Set the CMake flag `GTSAM_PYTHON_VERSION` to `3`, otherwise the default interpreter will be used.
  - You can do this on the command line as follows:

    ```sh
    cmake -DGTSAM_INSTALL_CYTHON_TOOLBOX=ON -DGTSAM_PYTHON_VERSION=3 ..
    ```

## INSTALL

- In the `example` directory, create the `build` directory and `cd` into it.
- Run `cmake ..`.
- Run `make`, and the wrapped module will be installed to a `cython` directory.
- Navigate to the `cython` directory and run `python setup.py install`.

## DOCUMENTATION

For more detailed information, please refer to the [tutorial](TUTORIAL.md).
