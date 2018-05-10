include(FindPackageHandleStandardArgs)

find_program(Sphinx_EXECUTABLE sphinx-build)
find_package_handle_standard_args(Sphinx
    REQUIRED_VARS Sphinx_EXECUTABLE)

if(TARGET cmakeserver::Sphinx)
    return()
endif()

add_executable(cmakeserver::Sphinx IMPORTED)
set_property(TARGET cmakeserver::Sphinx
	PROPERTY IMPORTED_LOCATION ${Sphinx_EXECUTABLE})
