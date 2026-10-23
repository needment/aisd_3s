cmake_minimum_required(VERSION 3.12)

project(SetLab LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(set_lab main.cpp)

if (MSVC)
    target_compile_options(set_lab PRIVATE /W4)
else()
    target_compile_options(set_lab PRIVATE -Wall -Wextra -Wconversion -Wpedantic)
endif()