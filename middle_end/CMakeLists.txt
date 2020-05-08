cmake_minimum_required(VERSION 3.15)
project(middle_end)

set(CMAKE_CXX_STANDARD 14)

add_executable(middle_end main.cpp libraries/diffur.h libraries/my_stack.h libraries/my_stack.cpp
        libraries/MyTree.h libraries/MyTree.cpp)