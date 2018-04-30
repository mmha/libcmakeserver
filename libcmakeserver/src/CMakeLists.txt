find_package(Boost REQUIRED system thread filesystem)
find_package(Coroutines REQUIRED)
find_package(nlohmann_json REQUIRED)

add_library(cmakeserver
    ../include/cmakeserver/cmake_protocol.hpp
    ../include/cmakeserver/cmakeserver.hpp
    ../include/cmakeserver/coroutines.hpp
    ../include/cmakeserver/ipc.hpp
    ../include/cmakeserver/use_boost_future.hpp
    cmake_protocol.cpp
    ipc.cpp
    $<$<BOOL:${WIN32}>:ipc_win32.cpp>)
    #$<$<BOOL:${UNIX}>:ipc_posix.cpp>)
add_library(cmakeserver::cmakeserver ALIAS cmakeserver)

set(binary_include_dir "${CMAKE_CURRENT_BINARY_DIR}/include/")
generate_export_header(cmakeserver
    EXPORT_FILE_NAME "${binary_include_dir}/cmakeserver/export.h")

target_include_directories(cmakeserver PUBLIC
    "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../include>"
    "$<BUILD_INTERFACE:${binary_include_dir}>"
    $<INSTALL_INTERFACE:include>)
target_compile_features(cmakeserver PUBLIC cxx_std_20)
target_compile_options(cmakeserver
    PUBLIC $<$<CXX_COMPILER_ID:Clang>:-fdiagnostics-color=always>)
target_compile_definitions(cmakeserver PUBLIC BOOST_THREAD_VERSION=5)
target_link_libraries(cmakeserver PUBLIC
    Boost::boost
    Boost::system
    Boost::thread
    Boost::filesystem
    cmakeserver::Coroutines
    CONAN_PKG::jsonformoderncpp)

install(TARGETS cmakeserver EXPORT cmakeserver
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib)

install(DIRECTORY ${binary_include_dir} DESTINATION .)
