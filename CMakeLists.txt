cmake_minimum_required(VERSION 3.0...4.0)
project(Flock)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Compiler / Linker options
if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    if (MSVC)
        add_compile_options(
                /MDd
        )
    else ()
        add_compile_options(
                -Wall
                -Wextra
                -Wpedantic
                -fsanitize=address
                -g
        )

        add_link_options(-fsanitize=address)
    endif ()
endif ()

if (CMAKE_BUILD_TYPE STREQUAL "Release")
    if (MSVC)
        add_compile_options(
                /MD
        )
    else ()
        add_compile_options(
                -O3
        )
    endif ()
endif ()

include(cmake/CPM.cmake)
enable_testing()

add_subdirectory(Core)
add_subdirectory(Sandbox)