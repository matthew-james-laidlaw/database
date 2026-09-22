add_executable(db_tests
    db_tests.cpp
)

target_link_libraries(db_tests GTest::gtest_main database)
