cmake_minimum_required(VERSION 3.15)
project(backend_x86_64)

set(CMAKE_CXX_STANDARD 14)

IF(CMAKE_BUILD_TYPE MATCHES Debug)
    message("Debug build.")
    set(GCC_COVERAGE_COMPILE_FLAGS "-g -Wall -Wextra -Weffc++ -Wc++0x-compat -Wc++11-compat \
 -Wc++14-compat -Waggressive-loop-optimizations -Walloc-zero -Walloca -Walloca-larger-than=8192\
 -Warray-bounds -Wcast-align -Wcast-qual -Wchar-subscripts -Wconditionally-supported -Wconversion\
 -Wctor-dtor-privacy -Wdangling-else -Wduplicated-branches -Wempty-body -Wfloat-equal\
 -Wformat-nonliteral -Wformat-security -Wformat-signedness -Wformat=2 -Wformat-overflow=2\
 -Wformat-truncation=2 -Winline -Wlarger-than=8192 -Wvla-larger-than=8192 -Wlogical-op\
 -Wmissing-declarations -Wnon-virtual-dtor -Wopenmp-simd -Woverloaded-virtual -Wpacked\
 -Wpointer-arith -Wredundant-decls -Wrestrict -Wshadow -Wsign-promo -Wstack-usage=8192\
 -Wstrict-null-sentinel -Wstrict-overflow=2 -Wstringop-overflow=4 -Wsuggest-attribute=noreturn\
 -Wsuggest-final-types -Wsuggest-override -Wswitch-default -Wswitch-enum -Wsync-nand -Wundef\
 -Wunreachable-code -Wunused -Wvariadic-macros -Wno-literal-suffix -Wno-missing-field-initializers\
 -Wnarrowing -Wno-old-style-cast -Wvarargs -fcheck-new -fsized-deallocation -fstack-check\
 -fstrict-overflow -fchkp-first-field-has-own-bounds\
 -fchkp-narrow-to-innermost-array -flto-odr-type-merging")

    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${GCC_COVERAGE_COMPILE_FLAGS}" )

ELSEIF(CMAKE_BUILD_TYPE MATCHES Release)
    message("Release build.")
ENDIF()

add_executable(
        backend_x86_64 main.cpp
        src/base/my_tree.h
        src/base/my_tree.cpp

        src/backend_x86_64.h
        src/backend_x86_64.cpp

        src/name_table.h
        src/name_table.cpp

        src/bin_commands.h
        src/com_functions.cpp
        src/decl_com_functions.inc)

IF (WIN32)
    set(CMAKE_CXX_STANDARD_LIBRARIES "-static-libgcc -static-libstdc++
        -lwsock32 -lws2_32 ${CMAKE_CSS_STANDARD_LIBRARIES}")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Wl,-Bstatic,--whole-archive \
        -lwinpthread -Wl,--no-whole-archive")
ENDIF()