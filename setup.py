#!/usr/bin/env python

# Todo list to prepare a release:
#  - update VERSION in failmalloc.c and setup.py
#  - run unit tests: run tox
#  - set release date in the README.rst file
#  - git commit -a -m "prepare release x.y"
#  - git push
#
# Release a new version:
#
#  - git tag VERSION
#  - git push --tags
#  - python setup.py register sdist upload
#
# After the release:
#  - set version to n+1
#  - git commit -a -m "post-release"
#  - git push

try:
    from setuptools import setup, Extension
    SETUPTOOLS = True
except ImportError:
    SETUPTOOLS = False
    # Use distutils.core as a fallback.
    # We won't be able to build the Wheel file on Windows.
    from distutils.core import setup, Extension

VERSION = '0.3'

CLASSIFIERS = [
    'Development Status :: 3 - Alpha',
    'Intended Audience :: Developers',
    'License :: OSI Approved :: MIT License',
    'Natural Language :: English',
    'Operating System :: OS Independent',
    'Programming Language :: C',
    'Programming Language :: Python',
    'Topic :: Security',
    'Topic :: Software Development :: Debuggers',
    'Topic :: Software Development :: Libraries :: Python Modules',
]

def main():
    with open('README.rst') as f:
        long_description = f.read().strip()

    ext = Extension('failmalloc', ['failmalloc.c'])

    options = {
        'name': 'pyfailmalloc',
        'version': VERSION,
        'license': 'MIT license',
        'description': 'inject memory allocation faults',
        'long_description': long_description,
        'url': 'https://github.com/vstinner/pyfailmalloc',
        'author': 'Victor Stinner',
        'author_email': 'victor.stinner@gmail.com',
        'ext_modules': [ext],
        'classifiers': CLASSIFIERS,
    }
    setup(**options)

if __name__ == "__main__":
    main()

