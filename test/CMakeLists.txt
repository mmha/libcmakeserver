find_package(doctest REQUIRED)

add_executable(testrunner protocol/version.cpp main.cpp)
target_link_libraries(testrunner PRIVATE cmakeserver::cmakeserver cmakeserver::doctest)
