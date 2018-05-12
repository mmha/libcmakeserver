find_package(Sphinx)
find_package(Doxygen)

if(Doxygen_FOUND)
	set(doxy_dest "${CMAKE_CURRENT_BINARY_DIR}/doxyxml")
	file(MAKE_DIRECTORY ${doxy_dest})
	if(TARGET Doxygen::dot)
	    set(USE_DOT "YES")
	else()
	    set(USE_DOT "NO")
	endif()
	configure_file("${PROJECT_SOURCE_DIR}/Doxyfile.in" "${PROJECT_BINARY_DIR}/Doxyfile" @ONLY)
	add_custom_target(doxy_doc
	    Doxygen::doxygen  "${PROJECT_BINARY_DIR}/Doxyfile"
	    SOURCES           "${PROJECT_BINARY_DIR}/Doxyfile"
	    WORKING_DIRECTORY "${doxy_dest}")
endif()

if(Sphinx_FOUND)
	set(sphinx_root ${CMAKE_CURRENT_SOURCE_DIR})
	set(sphinx_dest ${CMAKE_CURRENT_BINARY_DIR})
	set(sphinx_author "Morris Hafner")
	string(TIMESTAMP sphinx_copyright "%Y, Morris Hafner")
	configure_file("conf.py" "conf.py" @ONLY)
	add_custom_target(sphinx_doc
		cmakeserver::Sphinx
			-c "${CMAKE_CURRENT_BINARY_DIR}" -j auto
			"${CMAKE_CURRENT_SOURCE_DIR}"
			"${CMAKE_CURRENT_BINARY_DIR}"
		SOURCES "${CMAKE_CURRENT_BINARY_DIR}/conf.py"
		USES_TERMINAL
	)
	add_dependencies(sphinx_doc doxy_doc)
endif()
