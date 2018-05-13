find_package(doctest REQUIRED)

add_executable(testrunner
    protocol/version.cpp protocol/hello.cpp
    system/temporary_filename.cpp
    main.cpp)
target_link_libraries(testrunner PRIVATE cmakeserver::cmakeserver cmakeserver::doctest)

add_executable(playground playground.cpp)
target_link_libraries(playground PRIVATE cmakeserver::cmakeserver)
