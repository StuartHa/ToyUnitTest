cmake_minimum_required(VERSION 3.14)
project(unit_test_toy)

set(CMAKE_CXX_STANDARD 17)

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()

add_executable(unit_test_toy example.cpp)
target_link_libraries(unit_test_toy ${CONAN_LIBS})