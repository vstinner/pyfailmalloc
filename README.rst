++++++++++++
pyfailmalloc
++++++++++++

Debug tool for Python injecting memory allocation faults to simulate a low
memory system to test how your application handles ``MemoryError`` exceptions.

pyfailmalloc is based on the `PEP 445
<http://www.python.org/dev/peps/pep-0445/>`_ "*Add new APIs to customize Python
memory allocators*" and so requires at least Python 3.4.

* `pyfailmalloc at the Python Cheeseshop (PyPI)
  <http://pypi.python.org/pypi/pyfailmalloc>`_
* `pyfailmalloc project at Bitbucket (source code)
  <https://bitbucket.org/vstinner/pyfailmalloc>`_


API
===

* ``failmalloc.enable(range: int=1000)``: schedule a memory allocation failure
  in random.randint(1, range) allocations.
* ``failmalloc.disable()``: cancel the scheduled memory allocation failure

The version can be read from failmalloc.__version__ as a string (ex:
``"0.1"``).


Changelog
=========

Version 0.2 (2015-01-27)

- Support Python 3.5: hook also calloc(), support the new PyMemAllocatorEx API
- Add tox.ini to run tests using tox

Version 0.1 (2013-07-08)

- First public version


See also
========

* `failmalloc <http://www.nongnu.org/failmalloc/>`_
* `pytracemalloc <http://pypi.python.org/pypi/pytracemalloc>`_

