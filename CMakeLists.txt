cmake_minimum_required(VERSION 3.5)
project(pixf)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

option(GLFW_BUILD_DOCS OFF)
option(GLFW_BUILD_EXAMPLES OFF)
option(GLFW_BUILD_TESTS OFF)
add_subdirectory(${PROJECT_SOURCE_DIR}/vendor/glfw)
add_subdirectory(${PROJECT_SOURCE_DIR}/vendor/glad)
add_subdirectory(${PROJECT_SOURCE_DIR}/vendor/stbi)
add_subdirectory(${PROJECT_SOURCE_DIR}/vendor/glm)
add_subdirectory(${PROJECT_SOURCE_DIR}/vendor/assimp)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/out/lib/${CMAKE_SYSTEM_NAME}-${CMAKE_SYSTEM_PROCESSOR}-${CMAKE_BUILD_TYPE})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/out/bin/${CMAKE_SYSTEM_NAME}-${CMAKE_SYSTEM_PROCESSOR}-${CMAKE_BUILD_TYPE})

find_package(OpenGL REQUIRED)

set(VENDOR_PATH
        ${PROJECT_SOURCE_DIR}/vendor
)

file(GLOB_RECURSE PROJECT_SOURCES CONFIGURE_DEPENDS
        "${CMAKE_CURRENT_LIST_DIR}/include/*.h"
        "${CMAKE_CURRENT_LIST_DIR}/src/*.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/src/*.h"
)


set(EXAMPLE_SOURCES
        examples/example.cpp
)

add_library(${PROJECT_NAME} STATIC
        ${PROJECT_SOURCES}
)

target_link_libraries(${PROJECT_NAME}
        PRIVATE
        OpenGL::GL
        glfw
        glad
        stbi
        glm
        assimp
)

target_include_directories(${PROJECT_NAME} PRIVATE
        src
        include
        ${VENDOR_PATH}/glfw/include
        ${VENDOR_PATH}/glad/include
        ${VENDOR_PATH}/stbi/include
        ${VENDOR_PATH}/glm/glm
        ${VENDOR_PATH}/assimp/include
)

if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    if (MSVC)
        target_compile_options(${PROJECT_NAME} PRIVATE
                /W4
                /Zi
                /RTC1
        )
    else ()
        target_compile_options(${PROJECT_NAME} PRIVATE
                -Wall -Wextra -Wpedantic
                -g3
        )
    endif ()
endif ()

if (CMAKE_BUILD_TYPE STREQUAL "Release")
    if (MSVC)
        target_compile_options(${PROJECT_NAME} PRIVATE
                /O2
                /GL
                /fp:fast
        )
        target_link_options(${PROJECT_NAME} PRIVATE
                /LTCG
        )
    else ()
        target_compile_options(${PROJECT_NAME} PRIVATE
                -O3
                -march=native
                -DNDEBUG
        )
    endif ()
endif ()


# macOS libs
if (APPLE)
    find_library(COCOA_LIBRARY Cocoa REQUIRED)
    find_library(IOKIT_LIBRARY IOKit REQUIRED)
    find_library(COREVIDEO_LIBRARY CoreVideo REQUIRED)
    target_link_libraries(${PROJECT_NAME} PRIVATE
            ${COCOA_LIBRARY}
            ${IOKIT_LIBRARY}
            ${COREVIDEO_LIBRARY}
    )
    # Linux libs
elseif (UNIX AND NOT APPLE)
    find_package(PkgConfig REQUIRED)
    pkg_check_modules(X11 REQUIRED x11)
    target_link_libraries(${PROJECT_NAME} PRIVATE ${X11_LIBRARIES})
endif ()

add_executable(${PROJECT_NAME}_example ${EXAMPLE_SOURCES})
target_link_libraries(${PROJECT_NAME}_example ${PROJECT_NAME} glm)
target_include_directories(${PROJECT_NAME}_example PRIVATE
        src
        include
        ${VENDOR_PATH}/glm/glm
)
