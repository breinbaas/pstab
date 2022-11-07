python_example
==============

[![Gitter][gitter-badge]][gitter-link]

|      CI              | status |
|----------------------|--------|
| Linux/macOS Travis   | [![Travis-CI][travis-badge]][travis-link] |
| MSVC 2015            | [![AppVeyor][appveyor-badge]][appveyor-link] |
| conda.recipe         | [![Conda Actions Status][actions-conda-badge]][actions-conda-link] |
| pip builds           | [![Pip Actions Status][actions-pip-badge]][actions-pip-link] |
| [`cibuildwheel`][]   | [![Wheels Actions Status][actions-wheels-badge]][actions-wheels-link] |

[gitter-badge]:            https://badges.gitter.im/pybind/Lobby.svg
[gitter-link]:             https://gitter.im/pybind/Lobby
[actions-badge]:           https://github.com/breinbaas/pstab/workflows/Tests/badge.svg
[actions-conda-link]:      https://github.com/breinbaas/pstab/actions?query=workflow%3A%22Conda
[actions-conda-badge]:     https://github.com/breinbaas/pstab/workflows/Conda/badge.svg
[actions-pip-link]:        https://github.com/breinbaas/pstab/actions?query=workflow%3A%22Pip
[actions-pip-badge]:       https://github.com/breinbaas/pstab/workflows/Pip/badge.svg
[actions-wheels-link]:     https://github.com/breinbaas/pstab/actions?query=workflow%3AWheels
[actions-wheels-badge]:    https://github.com/breinbaas/pstab/workflows/Wheels/badge.svg
[travis-link]:             https://travis-ci.org/breinbaas/pstab
[travis-badge]:            https://travis-ci.org/breinbaas/pstab.svg?branch=master&status=passed
[appveyor-link]:           https://ci.appveyor.com/project/breinbaas/python-pstab
<!-- TODO: get a real badge link for appveyor -->
[appveyor-badge]:          https://travis-ci.org/breinbaas/pstab.svg?branch=master&status=passed

A package to calculate slope stability safety factors.
This requires Python 3.7+; for older versions of Python, check the commit
history.

Installation
------------

 - clone this repository
 - `pip install ./pstab`

CI Examples
-----------

There are examples for CI in `.github/workflows`. A simple way to produces
binary "wheels" for all platforms is illustrated in the "wheels.yml" file,
using [`cibuildwheel`][]. You can also see a basic recipe for building and
testing in `pip.yml`, and `conda.yml` has an example of a conda recipe build.


Building the documentation
--------------------------

Documentation for the example project is generated using Sphinx. Sphinx has the
ability to automatically inspect the signatures and documentation strings in
the extension module to generate beautiful documentation in a variety formats.
The following command generates HTML-based reference documentation; for other
formats please refer to the Sphinx manual:

 - `cd python_example/docs`
 - `make html`

License
-------

pybind11 is provided under a BSD-style license that can be found in the LICENSE
file. By using, distributing, or contributing to this project, you agree to the
terms and conditions of this license.

Test call
---------

```python
import pstab
pstab.bishop("TODO")
```

[`cibuildwheel`]:          https://cibuildwheel.readthedocs.io
