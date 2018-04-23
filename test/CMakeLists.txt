add_executable(testrunner main.cpp)
target_link_libraries(testrunner PRIVATE cmakeserver::cmakeserver)
