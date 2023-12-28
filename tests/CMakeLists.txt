cmake_minimum_required(VERSION 3.14)
project(BadgerVault)

# GoogleTest requires at least C++14
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()

# How to run the tests
# 1) Add the test files in add_executable
# 2) Add the test file in target_link_libraries
# 3) Create an gtest_discover_tests(file_test)
# 4) Run the following command cmake -S . -B build && cmake --build build && cd build && ctest


# Add the names of the files you want to execute
add_executable(
  pagetests
  pagetests.cpp
  ../Database/page.cpp  # Ensure this is included!!!
)

target_link_libraries(
  pagetests
  GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(pagetests)
