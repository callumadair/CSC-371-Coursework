cmake_minimum_required(VERSION 3.22)
project(CSC_371_Coursework)

set(CMAKE_CXX_STANDARD 14)

add_executable(CSC_371_Coursework
        src/main.cpp
        src/371pass.cpp src/371pass.h
        src/category.cpp src/category.h
        src/item.cpp src/item.h
        src/lib_catch.hpp src/lib_catch_main.cpp
        src/lib_cxxopts.hpp src/lib_cxxopts_argv.hpp
        src/lib_json.hpp
        src/wallet.cpp src/wallet.h)

add_executable(CSC_371_Coursework_Testall
        src/main.cpp
        src/371pass.cpp src/371pass.h
        src/category.cpp src/category.h
        src/item.cpp src/item.h
        src/lib_catch.hpp src/lib_catch_main.cpp
        src/lib_cxxopts.hpp src/lib_cxxopts_argv.hpp
        src/lib_json.hpp
        src/wallet.cpp src/wallet.h
        tests/testall.cpp
        tests/test1.cpp tests/test2.cpp tests/test3.cpp
        tests/test4.cpp tests/test5.cpp tests/test6.cpp
        tests/test7.cpp tests/test8.cpp tests/test9.cpp
        tests/test10.cpp tests/test11.cpp)
