cmake_minimum_required(VERSION 3.5)
project(glad)

add_library(${PROJECT_NAME} STATIC
        src/glad.c
)

target_include_directories(glad PRIVATE include)