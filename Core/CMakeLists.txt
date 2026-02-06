project(FlockCore)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/out/lib/${CMAKE_SYSTEM_NAME}-${CMAKE_SYSTEM_PROCESSOR}-${CMAKE_BUILD_TYPE})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/out/lib/${CMAKE_SYSTEM_NAME}-${CMAKE_SYSTEM_PROCESSOR}-${CMAKE_BUILD_TYPE})
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

find_package(OpenGL REQUIRED)

CPMAddPackage(
        URI "gh:glfw/glfw#3.4"
        OPTIONS
        "GLFW_BUILD_DOCS OFF"
        "GLFW_BUILD_TESTS OFF"
        "GLFW_BUILD_EXAMPLES OFF"
)

CPMAddPackage(
        NAME glad
        GITHUB_REPOSITORY Pixels67/glad
        GIT_TAG master
)

CPMAddPackage(
        NAME SLog
        GITHUB_REPOSITORY Pixels67/SLog
        GIT_TAG master
)

set(PROJECT_SOURCES
        Pch.hpp
        include/Flock.hpp
        src/Flock.cpp
        src/Common.hpp
        src/Debug/Log.hpp
)

add_library(${PROJECT_NAME} SHARED ${PROJECT_SOURCES})

target_precompile_headers(${PROJECT_NAME} PUBLIC Pch.hpp)
target_include_directories(${PROJECT_NAME} PUBLIC include src)
target_compile_definitions(${PROJECT_NAME} PRIVATE FLK_DYLIB_BUILD)

target_link_libraries(${PROJECT_NAME} PUBLIC
        OpenGL::GL
        glfw
        glad
        SLog
)

# OS libs
if (APPLE)
    find_library(COCOA_LIBRARY Cocoa REQUIRED)
    find_library(IOKIT_LIBRARY IOKit REQUIRED)
    find_library(COREVIDEO_LIBRARY CoreVideo REQUIRED)
    target_link_libraries(${PROJECT_NAME} PRIVATE
            ${COCOA_LIBRARY}
            ${IOKIT_LIBRARY}
            ${COREVIDEO_LIBRARY}
    )
elseif (UNIX AND NOT APPLE)
    find_package(PkgConfig REQUIRED)
    pkg_check_modules(X11 REQUIRED x11)
    target_link_libraries(${PROJECT_NAME} PRIVATE ${X11_LIBRARIES})
elseif (WIN32)
    target_link_libraries(${PROJECT_NAME} PRIVATE DbgHelp)
endif ()

if (NOT MSVC)
    target_compile_options(${PROJECT_NAME} PRIVATE -fvisibility=hidden)
endif ()