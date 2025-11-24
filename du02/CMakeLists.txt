cmake_minimum_required(VERSION 3.28)
project(du02)

set(CMAKE_CXX_STANDARD 20)

include_directories(.)

set(SOURCE_FILES
        Tree.cpp
        Tree.h
        felsenstein.cpp
        felsenstein.h
        alignment.cpp
        alignment.h
)

add_executable(test ${SOURCE_FILES} test.cpp)

add_custom_command(TARGET test PRE_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy "${CMAKE_SOURCE_DIR}/tree.txt"
                "${CMAKE_SOURCE_DIR}/cftr.txt"
        #        ${CMAKE_SOURCE_DIR}/The_Matrix.srt
        $<TARGET_FILE_DIR:test>
        VERBATIM)
