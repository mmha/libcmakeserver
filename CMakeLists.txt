cmake_minimum_required(VERSION 3.11)
project(libcmakeserver VERSION 0.1)

if(EXISTS "${CMAKE_BINARY_DIR}/conanbuildinfo.cmake")
    include("${CMAKE_BINARY_DIR}/conanbuildinfo.cmake")
    conan_basic_setup(TARGETS NO_OUTPUT_DIRS)
endif()

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
include(CMakePackageConfigHelpers)
include(GenerateExportHeader)

set(CMAKE_CXX_VISIBILITY_PRESET hidden)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_VISIBILITY_INLINES_HIDDEN ON)

add_subdirectory(doc)
add_subdirectory(libcmakeserver)

enable_testing()
add_subdirectory(test)

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
