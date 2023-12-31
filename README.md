# Pendiction

## Table of Contents
1. [TLDR](#tldr)
2. [Introduction](#introduction)
3. [Approach](#approach)
4. [Testing](#testing)

## TLDR

## Introduction

## Approach

## Testing
For the testing, we will be using the [Google Testing](https://google.github.io/googletest/) library. In order to set up your own tests, the following must be done. 

1. Create all tests within the `./tests` folder. Make sure to use Google Testing library
2. Once you have created the tests, then add your test in `CMakeLists.txt`
```cpp
add_executable(
  your_test_file
  your_test_file.cpp
  // Include any classes file directory here
)
```
3. Add this to the end of the file for google tests to discover `gtest_discover_tests(your_test_file)`
4. Run the following command `cmake -S . -B build && cmake --build build && cd build && ctest`
