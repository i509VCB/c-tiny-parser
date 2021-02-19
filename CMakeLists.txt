cmake_minimum_required(VERSION 3.17)
project(c_tiny_parser C)

enable_testing()

set(CMAKE_C_STANDARD 99)

add_library(c_tiny_parser tiny.c tiny.h)

add_executable(c_tiny_parser_test test.c)

target_link_libraries(c_tiny_parser_test c_tiny_parser)

add_test(
        NAME c_tiny_parser_test
        COMMAND c_tiny_parser_test
        ${CMAKE_CURRENT_LIST_DIR}/test.tiny
)
