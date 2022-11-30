cmake_minimum_required(VERSION 3.0.0)
project(shared_memory VERSION 0.1.0)

include(CTest)
enable_testing()

add_executable(shared_memory main.cpp)
add_executable(read read.cpp)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
