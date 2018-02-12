# eventfall
Concurrent graph-based task system. Library in early stage of development (no examples, docs for now).

 Licence
==========
Apache License 2.0

2 Prerequisites
===============
- C++17 
- Conan
- QBS
- [IdealCouscous](https://github.com/maxis11/ideal-couscous-conan-package). Export to zatm8/testing using conan.
- TBB (from 3rdparty). Export to zatm8/stable using conan.
- libcds (from 3rdparty). Export to zatm8/testing using conan.

3 Building
===============
In conanfile.txt after section [imports] you must change to build directory and append "/lib" to the end.
Build 3rdparty dependencies using conan. (conan export . <zatm8/*> for each dep (in dep's folder) and then conan install . --build=missing (in project folder) ).
After that you can build library and tests using qbs.

4 Examples
===============
For now you can find it in /internal_tests/node_playground/main.cpp (It will change constantly cause I'm using it as playground).
