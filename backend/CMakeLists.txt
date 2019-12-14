cmake_minimum_required(VERSION 3.15)
project(backend)

set(CMAKE_CXX_STANDARD 14)

add_executable(backend main.cpp libraries/my_stack.h libraries/my_stack.cpp libraries/MyTree.h libraries/MyTree.cpp libraries/diffur.h)