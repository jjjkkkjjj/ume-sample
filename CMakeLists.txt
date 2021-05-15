# configure cmake version
cmake_minimum_required(VERSION 2.8)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# project name
project(test C CXX)

set(INC_PATH "/usr/local/include" CACHE STRING "The include path")
set(LIB_PATH "/usr/local/lib" CACHE STRING "The library path")

# to include header files
include_directories(${test_SOURCE_DIR}/umesimd)

# create static library named `add`
#add_library(add STATIC add.c)

# create test.out from test.cpp
add_executable(test.out test.cpp)

# link add.a to addTest.out
#target_link_libraries(addTest.out add)
# link gtest and gtest_main to addTest.out
#target_link_libraries(addTest.out gtest gtest_main)
target_include_directories(test.out PUBLIC ${INC_PATH})