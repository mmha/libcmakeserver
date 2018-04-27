libcmakeserver
==============

libcmakeserver is a wrapper library to connect to a CMake server instance for C++.
It makes use of modern C++17 features and the Coroutines TS.

Requirements
------------
- clang 6.0
- CMake 3.12
- Boost >=1.67.0
- optional: conan

Building
--------

### Conan
```bash
conan create . user/channel
```

### CMake
Build the library itself:
```bash
mkdir build
cd build
cmake .. -GNinja -DCMAKE_BUILD_TYPE=Release
ninja install
```
Use it in your project:
```cmake
find_package(cmakeserver REQUIRED)
target_link_libraries(myapp PRIVATE cmakeserver::cmakeserver)
```

License
-------
This project is licensed under the Boost Software License v1.0.
