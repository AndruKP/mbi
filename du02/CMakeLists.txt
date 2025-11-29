cmake_minimum_required(VERSION 3.28)
project(du02)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS -O2)

include_directories(.)

set(SOURCE_FILES
        tree.cpp
        tree.h
        felsenstein.cpp
        felsenstein.h
        alignment.cpp
        alignment.h
)

add_executable(test ${SOURCE_FILES} test.cpp)

add_custom_command(TARGET test PRE_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy "${CMAKE_SOURCE_DIR}/tree.txt"
        "${CMAKE_SOURCE_DIR}/cftr.txt"
        $<TARGET_FILE_DIR:test>
        VERBATIM)
