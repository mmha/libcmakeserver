libcmakeserver
==============

libcmakeserver is a wrapper library to connect to a CMake server instance for C++.
It makes use of modern C++17 features and the Coroutines TS.

Requirements
------------
- clang ≥6.0
- CMake ≥3.12
- conan, or:
	- Boost ≥1.67.0
	- nlohmann/json
	- doctest

Building
--------

Conan
~~~~~

.. highlight:: bash
.. code:: 

	conan create . user/channel --build missing

CMake
~~~~~
Build the library itself:

.. highlight:: bash
.. code::

	mkdir build
	cd build
	cmake .. -GNinja -DCMAKE_BUILD_TYPE=Release
	ninja install

Use it in your project:

.. highlight:: cmake
.. code::

	find_package(cmakeserver REQUIRED)
	target_link_libraries(myapp PRIVATE cmakeserver::cmakeserver)

License
-------
This project is licensed under the Boost Software License v1.0.

Contents
--------

.. toctree::
   :maxdepth: 3

   API/index