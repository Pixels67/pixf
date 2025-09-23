cmake_minimum_required(VERSION 3.5)
project(stbi)

add_library(${PROJECT_NAME} STATIC
        include/stb_image.h
        src/stb_image.cpp
)

target_include_directories(${PROJECT_NAME} PRIVATE include)