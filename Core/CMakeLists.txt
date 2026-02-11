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
        src/Ecs/Storage.hpp
        src/Common.hpp
        src/Debug/Log.hpp
        src/Ecs/Entity.hpp
        src/Ecs/Registry.cpp
        src/Ecs/Registry.hpp
        src/TypeId.hpp
        src/TypeId.cpp
        src/Ecs/Schedule.cpp
        src/Ecs/Schedule.hpp
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

################## Tests ##################

CPMAddPackage("https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip")
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

add_executable(${PROJECT_NAME}Tests
        tests/Ecs.cpp
)

target_link_libraries(${PROJECT_NAME}Tests
        GTest::gtest_main
        ${PROJECT_NAME}
)

include(GoogleTest)
gtest_discover_tests(${PROJECT_NAME}Tests)

################## OS Libs ##################

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