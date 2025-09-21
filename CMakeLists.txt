cmake_minimum_required(VERSION 3.5)
project(engine)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

option(GLFW_BUILD_DOCS OFF)
option(GLFW_BUILD_EXAMPLES OFF)
option(GLFW_BUILD_TESTS OFF)
add_subdirectory(${PROJECT_SOURCE_DIR}/vendor/glfw)
add_subdirectory(${PROJECT_SOURCE_DIR}/vendor/glad)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/out/lib/${CMAKE_SYSTEM_NAME}-${CMAKE_SYSTEM_PROCESSOR}-${CMAKE_BUILD_TYPE})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/out/bin/${CMAKE_SYSTEM_NAME}-${CMAKE_SYSTEM_PROCESSOR}-${CMAKE_BUILD_TYPE})

find_package(OpenGL REQUIRED)

set(VENDOR_PATH
        ${PROJECT_SOURCE_DIR}/vendor
)

set(PROJECT_SOURCES
        include/engine.h
        src/engine.cpp
        src/graphics/graphics.h
        src/graphics/shader.cpp
        src/graphics/shader.h
)

set(EXAMPLE_SOURCES
        src/examples/example.cpp
)

add_library(${PROJECT_NAME} STATIC
        ${PROJECT_SOURCES}
)

target_link_libraries(${PROJECT_NAME}
        PRIVATE
        OpenGL::GL
        glfw
        glad
)

target_include_directories(${PROJECT_NAME} PRIVATE
        src
        include
        ${VENDOR_PATH}/glfw/include
        ${VENDOR_PATH}/glad/include
)


# macOS libs
if(APPLE)
    find_library(COCOA_LIBRARY Cocoa REQUIRED)
    find_library(IOKIT_LIBRARY IOKit REQUIRED)
    find_library(COREVIDEO_LIBRARY CoreVideo REQUIRED)
    target_link_libraries(${PROJECT_NAME} PRIVATE
            ${COCOA_LIBRARY}
            ${IOKIT_LIBRARY}
            ${COREVIDEO_LIBRARY}
    )
# Linux libs
elseif(UNIX AND NOT APPLE)
    find_package(PkgConfig REQUIRED)
    pkg_check_modules(X11 REQUIRED x11)
    target_link_libraries(${PROJECT_NAME} PRIVATE ${X11_LIBRARIES})
endif()

add_executable(${PROJECT_NAME}_example ${EXAMPLE_SOURCES})
target_link_libraries(${PROJECT_NAME}_example ${PROJECT_NAME})
target_include_directories(${PROJECT_NAME}_example PRIVATE
        src
        include
)