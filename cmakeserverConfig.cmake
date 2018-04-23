include(CMakeFindDependencyMacro)
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}")
find_dependency(Boost COMPONENTS system thread)
find_dependency(Coroutines)

include("${CMAKE_CURRENT_LIST_DIR}/cmakeserver.cmake")
