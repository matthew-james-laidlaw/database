cmake_minimum_required(VERSION 3.29.6)

project(DATABASE LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/bin)

include(FetchContent)

FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG        v1.18.0
)

FetchContent_MakeAvailable(googletest)

add_subdirectory(apps)

add_library(db
    src/database.cpp
    src/evaluator.cpp
    src/lexer.cpp
    src/parser.cpp
    src/statement.cpp
    src/token.cpp
)

target_include_directories(db PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src/include)

add_executable(db_tests
    test/db_tests.cpp
)

target_link_libraries(db_tests GTest::gtest_main db)
