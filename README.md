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
- [IdealCouscous](https://github.com/maxis11/ideal-couscous-conan-package). Export in the zatm8/testing using conan.
- TBB (from 3rdparty). Export in the zatm8/stable using conan.
- libcds (from 3rdparty). Export in the zatm8/testing using conan.

3 Building
===============
In conanfile.txt after section [imports] you must change build directory path and append "/lib" at the end.
Build the 3rdparty dependencies using conan. (conan export . <zatm8/*> for each dep (in a dep's folder) and then conan install . --build=missing (in the project folder) ).
After that you can build the library and tests using qbs.

4 Examples
===============
For now you can find it in [internal_tests/node_playground/main.cpp](https://github.com/maxis11/eventfall/tree/master/tests/internal_tests/node_playground/main.cpp) (It will change constantly cause I'm using it as a playground).

4 TODO
===============
1. Task package
2. Simple scheduler
3. Thread pool
4. Events
5. Cling tasks
6. Python tasks
7. Examples
8. Documentation
