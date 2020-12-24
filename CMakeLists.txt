cmake_minimum_required(VERSION 3.11.4)

project(rlc C)

# Build all files in src/
file(GLOB_RECURSE SOURCES "src/*.c")

add_executable(rlc ${SOURCES})
