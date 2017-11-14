cmake_minimum_required(VERSION 3.6)
project(compiler)

set(CMAKE_CXX_STANDARD 11)
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}  -Wall -lasan -O2 -g -Wall -Wextra -Waggressive-loop-optimizations -Wcast-align -Wcast-qual -Wchar-subscripts -Wempty-body -Wfloat-equal -Wformat-nonliteral -Wformat-security -Wformat-signedness -Wformat=2 -Winline -Wlarger-than=8192 -Wlogical-op -Wmissing-declarations -Wopenmp-simd -Wpacked -Wpointer-arith -Wredundant-decls -Wshadow -Wsign-conversion -Wstack-usage=8192 -Wstrict-overflow=2 -Wsuggest-attribute=noreturn -Wsuggest-final-methods -Wsuggest-final-types -Wswitch-default -Wswitch-enum -Wsync-nand -Wundef -Wunreachable-code -Wunused -Wvariadic-macros -Wno-missing-field-initializers -Wno-narrowing -Wno-varargs -fcheck-new -fstack-check -fstack-protector -fstrict-overflow -fchkp-first-field-has-own-bounds -fchkp-narrow-to-innermost-array -flto-odr-type-merging -fno-omit-frame-pointer -fsanitize=address -fsanitize=alignment -fsanitize=bool -fsanitize=bounds -fsanitize=enum -fsanitize=float-cast-overflow -fsanitize=float-divide-by-zero -fsanitize=integer-divide-by-zero -fsanitize=leak -fsanitize=nonnull-attribute -fsanitize=null -fsanitize=object-size -fsanitize=return -fsanitize=returns-nonnull-attribute -fsanitize=shift -fsanitize=signed-integer-overflow -fsanitize=undefined -fsanitize=unreachable -fsanitize=vla-bound -fsanitize=vptr -fPIE -pie")
include_directories("/home/fedya/Рабочий стол/my_include")
set(SOURCE_FILES
        cmake-build-debug/cmake_install.cmake
        cmake-build-debug/CMakeCache.txt
        cmake-build-debug/Makefile
        CMakeLists.txt
        commands.h
        enum_types.h
        gets.h
        list_functions.h
        main.cpp
        Makefile
        printff.asm
        proga.asm
        README.md
        supercmd.h
        to_asm_functions.h
        to_new_asm_functions.h
        Tree_Functions.h
        types.h
        types_with_code.h)

add_executable(compiler ${SOURCE_FILES})