cmake_minimum_required(VERSION 3.14)

project(bankManagementSystem)

set(CMAKE_CXX_STANDARD 17)

include_directories(include)
include_directories(src)

file(GLOB_RECURSE SOURCES "src/*.cpp" "include/*.hpp")

add_executable(main ${SOURCES})

# # 1. 下载 GoogleTest
include(FetchContent)
FetchContent_Declare(
  googletest
  URL http://cdn.tarsyun.com/src/release-1.11.0.tar.gz
  DOWNLOAD_EXTRACT_TIMESTAMP true
)
FetchContent_MakeAvailable(googletest)

# # 2. 开启测试功能
enable_testing()

# # 3. 添加测试可执行文件 (比如叫 run_tests)
add_executable(run_tests tests/test_main.cpp tests/test_linklist.cpp tests/test_application.cpp)

# # 4. 链接库 (把你的代码和测试框架连起来)
target_link_libraries(run_tests gtest_main)

# # 5. 添加 GoogleTest 的测试到 CTest
include(GoogleTest)
gtest_discover_tests(run_tests)