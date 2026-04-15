cmake_minimum_required(VERSION 3.0...4.0)
project(Flock)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Compiler / Linker options
if (MSVC)
    add_compile_options(
            /DNOMINMAX
            /DWIN32_LEAN_AND_MEAN
    )
elseif (CMAKE_CXX_COMPILER_ID MATCHES "Clang" AND NOT CMAKE_CXX_COMPILER_ID MATCHES "AppleClang")
    add_link_options(-fuse-ld=lld)
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    add_compile_options(-Wno-changes-meaning)
endif ()

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
set(FETCHCONTENT_QUIET OFF)
enable_testing()

add_subdirectory(Core)
add_subdirectory(Sandbox)