add_library(database
    database.cpp
    evaluator.cpp
    lexer.cpp
    parser.cpp
    statement.cpp
    token.cpp
)

target_include_directories(database
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(database logger)
