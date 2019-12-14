cmake_minimum_required(VERSION 3.15)
project(frontend_back)

set(CMAKE_CXX_STANDARD 14)

add_executable(frontend_back main.cpp libraries/my_stack.cpp libraries/my_stack.h libraries/MyTree.h
        libraries/MyTree.cpp)