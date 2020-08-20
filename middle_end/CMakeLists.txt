cmake_minimum_required(VERSION 3.15)
project(middle_end)

set(CMAKE_CXX_STANDARD 14)

add_executable(
        middle_end main.cpp
        libraries/my_stack.h
        libraries/my_stack.cpp

        libraries/MyTree.h
        libraries/MyTree.cpp

        libraries/diffur.h)

IF (WIN32)
    set(CMAKE_CXX_STANDARD_LIBRARIES "-static-libgcc -static-libstdc++
        -lwsock32 -lws2_32 ${CMAKE_CSS_STANDARD_LIBRARIES}")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Wl,-Bstatic,--whole-archive \
        -lwinpthread -Wl,--no-whole-archive")
ENDIF()