include(FindPackageHandleStandardArgs)

find_path(doctest_INCLUDE_DIRS doctest.h)
find_package_handle_standard_args(doctest
    REQUIRED_VARS doctest_INCLUDE_DIRS)

if(TARGET cmakeserver::doctest)
    return()
endif()

add_library(cmakeserver::doctest INTERFACE IMPORTED)
target_include_directories(cmakeserver::doctest INTERFACE ${doctest_INCLUDE_DIRS})
