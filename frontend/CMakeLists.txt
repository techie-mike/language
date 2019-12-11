cmake_minimum_required(VERSION 3.15)
project(frontend)

set(CMAKE_CXX_STANDARD 14)

add_executable(frontend main.cpp libraries/token.cpp libraries/token.h libraries/com_language.h
        libraries/functions.cpp libraries/functions.h libraries/base/diffur.h
        libraries/base/MyTree.h libraries/base/MyTree.cpp libraries/base/my_stack.h libraries/base/my_stack.cpp)