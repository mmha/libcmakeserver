cmake_minimum_required(VERSION 3.11)
project(libcmakeserver VERSION 0.1)

if(EXISTS "${CMAKE_BINARY_DIR}/conanbuildinfo.cmake")
    include("${CMAKE_BINARY_DIR}/conanbuildinfo.cmake")
    conan_basic_setup(TARGETS NO_OUTPUT_DIRS)
endif()

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

find_package(Boost REQUIRED system thread)
find_package(Coroutines REQUIRED)
find_package(nlohmann_json REQUIRED)

add_library(cmakeserver INTERFACE)
add_library(cmakeserver::cmakeserver ALIAS cmakeserver)

target_include_directories(cmakeserver INTERFACE
    "$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>"
    $<INSTALL_INTERFACE:include>)
#target_compile_features(cmakeserver INTERFACE cxx_std_20)
target_compile_options(cmakeserver INTERFACE -std=c++2a)
target_compile_definitions(cmakeserver INTERFACE BOOST_THREAD_VERSION=5)
target_link_libraries(cmakeserver INTERFACE
    Boost::boost
    Boost::system
    Boost::thread
	cmakeserver::Coroutines
	CONAN_PKG::jsonformoderncpp
	c++
	c++abi)

enable_testing()
add_subdirectory(test)

install(TARGETS cmakeserver EXPORT cmakeserver)
install(DIRECTORY include DESTINATION .)

include(CMakePackageConfigHelpers)
write_basic_package_version_file(
	"${CMAKE_CURRENT_BINARY_DIR}/cmakeserverConfigVersion.cmake"
	VERSION ${cmakeserver_VERSION_MAJOR}
	COMPATIBILITY SameMajorVersion
)

install(FILES
	cmakeserverConfig.cmake
	"${CMAKE_CURRENT_BINARY_DIR}/cmakeserverConfigVersion.cmake"
	"${PROJECT_SOURCE_DIR}/cmake/FindCoroutines.cmake"
	DESTINATION lib/cmakeserver
)
configure_file(cmakeserverConfig.cmake
	"${CMAKE_CURRENT_BINARY_DIR}/cmakeserverConfig.cmake"
	COPYONLY
)

export(EXPORT cmakeserver
	FILE "${CMAKE_CURRENT_BINARY_DIR}/cmakeserver.cmake"
	NAMESPACE cmakeserver::
)
install(EXPORT cmakeserver
	DESTINATION lib/cmakeserver
	NAMESPACE cmakeserver::
)
