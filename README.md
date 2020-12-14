# gtsam-project-python

Project template using GTSAM + Python wrapping

## PREREQUISITES

- Python 3.6+ is required, since we support these versions.
- To install the wrap package via `GTSAM`:

  - Set the CMake flag `GTSAM_BUILD_PYTHON` to `ON` to enable building the Pybind11 wrapper.
  - Set the CMake flag `GTSAM_PYTHON_VERSION` to `3.x` (e.g. `3.7`), otherwise the default interpreter will be used.
  - You can do this on the command line as follows:

    ```sh
    cmake -DGTSAM_BUILD_PYTHON=ON -DGTSAM_PYTHON_VERSION=3.7 ..
    ```
- Alternatively, you can install the wrap package directly from the [repo](https://github.com/borglab/wrap), but you will still need to install `GTSAM`.

## INSTALL

- In the top-level directory, create the `build` directory and `cd` into it.
- Run `cmake ..`.
- Run `make`, and the wrapped module will be installed to the `python` directory in the top-level.
- To install the wrapped module, simply run `make python-install`.
- You can also run `python main.py` which calls the wrapped module, to get a flavor of how it works.

## DOCUMENTATION

For more detailed information, please refer to the [tutorial](TUTORIAL.md).
