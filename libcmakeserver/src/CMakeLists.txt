find_package(Boost REQUIRED system thread)
find_package(Coroutines REQUIRED)
find_package(nlohmann_json REQUIRED)

add_library(cmakeserver
    ../include/cmakeserver/cmakeserver.hpp
    ../include/cmakeserver/coroutines.hpp
    ../include/cmakeserver/use_boost_future.hpp
    ../include/cmakeserver/cmake_protocol.hpp
    cmake_protocol.cpp)
add_library(cmakeserver::cmakeserver ALIAS cmakeserver)

target_include_directories(cmakeserver PUBLIC
    "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../include>"
    $<INSTALL_INTERFACE:include>)
#target_compile_features(cmakeserver PUBLIC cxx_std_17)
target_compile_options(cmakeserver
    PUBLIC -std=c++2a
    PRIVATE $<$<CXX_COMPILER_ID:Clang>:-fdiagnostics-color=always>)
target_compile_definitions(cmakeserver PUBLIC BOOST_THREAD_VERSION=5)
target_link_libraries(cmakeserver PUBLIC
    Boost::boost
    Boost::system
    Boost::thread
    cmakeserver::Coroutines
    CONAN_PKG::jsonformoderncpp
    c++
    c++abi)

install(TARGETS cmakeserver EXPORT cmakeserver
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib)
