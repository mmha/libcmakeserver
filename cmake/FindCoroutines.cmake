include(CheckCXXCompilerFlag)
include(CheckIncludeFileCXX)
include(FindPackageHandleStandardArgs)

check_cxx_compiler_flag(/await Coroutines_SUPPORTS_MS_FLAG)
check_cxx_compiler_flag(-fcoroutines-ts Coroutines_SUPPORTS_GNU_FLAG)
if(Coroutines_SUPPORTS_MS_FLAG OR Coroutines_SUPPORTS_GNU_FLAG)
    set(Coroutines_COMPILER_SUPPORT ON)
endif()

if(Coroutines_SUPPORTS_MS_FLAG)
    check_include_file_cxx("coroutine" Coroutines_STANDARD_LIBRARY_SUPPORT "/await")
    check_include_file_cxx("experimental/coroutine" Coroutines_EXPERIMENTAL_LIBRARY_SUPPORT "/await")
elseif(Coroutines_SUPPORTS_GNU_FLAG)
    check_include_file_cxx("coroutine" Coroutines_STANDARD_LIBRARY_SUPPORT "-fcoroutines-ts")
    check_include_file_cxx("experimental/coroutine" Coroutines_EXPERIMENTAL_LIBRARY_SUPPORT "-fcoroutines-ts")
endif()

if(Coroutines_EXPERIMENTAL_LIBRARY_SUPPORT OR Coroutines_STANDARD_LIBRARY_SUPPORT)
    set(Coroutines_LIBRARY_SUPPORT ON)
endif()

find_package_handle_standard_args(Coroutines
    REQUIRED_VARS Coroutines_LIBRARY_SUPPORT Coroutines_COMPILER_SUPPORT
    FAIL_MESSAGE "Verify that the compiler and the standard library both support the Coroutines TS")

if(NOT Coroutines_FOUND)
    return()
endif()

add_library(cmakeserver::Coroutines INTERFACE IMPORTED GLOBAL)
if(Coroutines_SUPPORTS_MS_FLAG)
    target_compile_options(cmakeserver::Coroutines INTERFACE /await)
elseif(Coroutines_SUPPORTS_GNU_FLAG)
    target_compile_options(cmakeserver::Coroutines INTERFACE -fcoroutines-ts)
endif()
