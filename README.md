Python wrappers for CMRep (Continuous Medial Representation)
============================================================

This project provides a Python interface for the [CMRep tools](https://github.com/pyushkevich/greedy). Currently, only a small portion of the cm-rep toolkit is exposed to Python.

Quick Start
-----------
Install the package:

```sh
pip install picsl_cmrep
```

Command-line tools exposed through the API can be accessed as follows:

```python
from picsl_cmrep import cmrep_vskel
cmrep_vskel('-h')
```
