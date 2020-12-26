cmake_minimum_required(VERSION 3.11.4)

project(rlc C)

#Exports compile commands - this can be used for language servers
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Build all files in src/
file(GLOB_RECURSE SOURCES "src/*.c")

add_executable(rlc ${SOURCES})

target_include_directories(rlc PUBLIC
    include
)
