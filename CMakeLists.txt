cmake_minimum_required(VERSION 3.6)
project(compiler)

set(CMAKE_CXX_STANDARD 11)
include_directories("/home/fedya/Рабочий стол/my_include")
set(SOURCE_FILES
        cmake-build-debug/cmake_install.cmake
        cmake-build-debug/CMakeCache.txt
        cmake-build-debug/Makefile
        Akinator_Functions.h
        CMakeLists.txt
        commands.h
        dump.gv
        dump.png
        dump_kvadr.png
        enum_types.h
        gets.h
        list_dump.gv
        list_dump.png
        list_functions.h
        main.cpp
        Makefile
        my_functions.h
        noproc_factorial.txt
        noproc_kvadr.txt
        printff.asm
        proga.asm
        README.md
        supercmd.h
        test.txt
        to_asm_functions.h
        to_new_asm_functions.h
        to_proc.txt
        Tree_Functions.h
        types.h
        types_with_code.h)

add_executable(compiler ${SOURCE_FILES})