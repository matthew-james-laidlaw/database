add_library(networking client.cpp server.cpp)
target_include_directories(networking PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(networking logger)
