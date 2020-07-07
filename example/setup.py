import os
import sys

try:
    from setuptools import setup, find_packages
except ImportError:
    from distutils.core import setup, find_packages

packages = find_packages()

package_data = {
    package: [
        f
        for f in os.listdir(package.replace(".", os.path.sep))
        if os.path.splitext(f)[1] in (".so", ".pyd")
    ]
    for package in packages
}

dependencies = ["Cython>=0.25.2", "backports_abc>=0.5", "numpy>=1.12.0"]

setup(
    name="gtsam_example",
    description="Simple example of wrapping projects with GTSAM",
    url="https://gtsam.org/",
    version="1.0.0",
    author="Varun Agrawal",
    author_email="varunagrawal@gatech.edu",
    license="Simplified BSD license",
    keywords="wrapper tutorial example",
    long_description="",
    long_description_content_type="text/markdown",
    python_requires=">=3.6",
    # https://pypi.org/pypi?%3Aaction=list_classifiers
    classifiers=[
        "Development Status :: 5 - Production/Stable",
        "Intended Audience :: Education",
        "Intended Audience :: Developers",
        "Intended Audience :: Science/Research",
        "Operating System :: MacOS",
        "Operating System :: Microsoft :: Windows",
        "Operating System :: POSIX",
        "License :: OSI Approved :: BSD License",
        "Programming Language :: Python :: 2",
        "Programming Language :: Python :: 3",
    ],
    packages=packages,
    # Load the built shared object files
    package_data=package_data,
    install_requires=dependencies,
)
