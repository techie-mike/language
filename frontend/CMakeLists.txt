cmake_minimum_required(VERSION 3.15)
project(frontend)

set(CMAKE_CXX_STANDARD 14)

add_executable(
        frontend main.cpp
        libraries/token.h
        libraries/token.cpp

        libraries/functions.h
        libraries/functions.cpp

        libraries/base/MyTree.h
        libraries/base/MyTree.cpp

        libraries/base/my_stack.h
        libraries/base/my_stack.cpp

        libraries/base/diffur.h
        libraries/com_language.h
        libraries/func_operators.h)

IF (WIN32)
    set(CMAKE_CXX_STANDARD_LIBRARIES "-static-libgcc -static-libstdc++
        -lwsock32 -lws2_32 ${CMAKE_CSS_STANDARD_LIBRARIES}")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Wl,-Bstatic,--whole-archive \
        -lwinpthread -Wl,--no-whole-archive")
ENDIF()