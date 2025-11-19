cmake_minimum_required(VERSION 3.10)

project(studentManagementSystem)

set(CMAKE_CXX_STANDARD 17)

include_directories(include)
include_directories(src)

file(GLOB SOURCES "src/*.cpp" "include/*.h")

add_executable(main ${SOURCES})