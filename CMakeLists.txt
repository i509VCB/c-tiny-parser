cmake_minimum_required(VERSION 3.17)
project(c_tiny_parser C)

set(CMAKE_C_STANDARD 99)

add_library(c_tiny_parser tiny.c tiny.h)
