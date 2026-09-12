cmake_minimum_required(VERSION 4.0.0)

project(DATABASE LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(db main.cpp)
