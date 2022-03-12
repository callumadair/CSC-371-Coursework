cmake_minimum_required(VERSION 3.22)
project(CSC_371_Coursework)

set(CMAKE_CXX_STANDARD 14)

add_executable(CSC_371_Coursework
        src/main.cpp
        src/371pass.cpp
        src/category.cpp
        src/item.cpp
        src/lib_catch.hpp src/lib_cxxopts.hpp src/lib_json.hpp
        src/wallet.cpp src/wallet.h)
