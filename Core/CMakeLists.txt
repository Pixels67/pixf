project(FlockCore)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/out/lib/${CMAKE_SYSTEM_NAME}-${CMAKE_SYSTEM_PROCESSOR}-${CMAKE_BUILD_TYPE})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/out/lib/${CMAKE_SYSTEM_NAME}-${CMAKE_SYSTEM_PROCESSOR}-${CMAKE_BUILD_TYPE})
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

find_package(OpenGL REQUIRED)

set(GLFW_BUILD_DOCS OFF)
set(GLFW_BUILD_TESTS OFF)
set(GLFW_BUILD_EXAMPLES OFF)
CPMAddPackage(
        NAME glfw
        GITHUB_REPOSITORY glfw/glfw
        GIT_TAG 3.4
        GIT_SHALLOW TRUE
)

CPMAddPackage(
        NAME glad
        GITHUB_REPOSITORY Pixels67/glad
        GIT_TAG master
        GIT_SHALLOW TRUE
)

CPMAddPackage(
        NAME SLog
        GITHUB_REPOSITORY Pixels67/SLog
        GIT_TAG master
        GIT_SHALLOW TRUE
)

CPMAddPackage(
        NAME assimp
        URL "https://github.com/assimp/assimp/archive/refs/tags/v6.0.2.tar.gz"
        OPTIONS
        "ASSIMP_BUILD_ALL_IMPORTERS_BY_DEFAULT OFF"
        "ASSIMP_BUILD_ALL_EXPORTERS_BY_DEFAULT OFF"
        "ASSIMP_BUILD_FBX_IMPORTER ON"
        "ASSIMP_BUILD_GLTF_IMPORTER ON"
        "ASSIMP_BUILD_OBJ_IMPORTER ON"
        "ASSIMP_BUILD_PLY_IMPORTER ON"
        "ASSIMP_BUILD_TESTS OFF"
)

add_library(stbi STATIC vendor/stbi.h vendor/stbi.c)

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
        src/Graphics/Buffer.hpp
        src/Memory/Buffer.hpp
        src/Memory/Buffer.cpp
        src/Graphics/Buffer.cpp
        src/Graphics/Gl.hpp
        src/Graphics/Gl.cpp
        src/Graphics/Shader.hpp
        src/Graphics/Shader.cpp
        src/Graphics/Pipeline.cpp
        src/Graphics/Pipeline.hpp
        src/Graphics/Image.hpp
        src/Math/Color.hpp
        src/Math/Utils.hpp
        src/Math/Utils.cpp
        src/Math/Matrix.hpp
        src/Math/Quaternion.hpp
        src/Math/Vector.hpp
        src/Math/Math.hpp
        src/Graphics/Texture2D.cpp
        src/Graphics/Texture2D.hpp
        src/Graphics/Mesh.cpp
        src/Graphics/Mesh.hpp
        src/Graphics/Vertex.hpp
        src/Graphics/VertexLayout.cpp
        src/Graphics/VertexLayout.hpp
        src/Graphics/VertexArray.cpp
        src/Graphics/VertexArray.hpp
        src/Glfw/Window.cpp
        src/Glfw/Window.hpp
        src/FileIo/Image.hpp
        src/FileIo/Image.cpp
        src/Asset/AssetLoader.hpp
        src/Graphics/Material.hpp
        src/FileIo/Pipeline.hpp
        src/FileIo/Pipeline.cpp
        src/FileIo/File.hpp
        src/FileIo/File.cpp
        src/Graphics/Renderer.cpp
        src/Graphics/Renderer.hpp
        src/Math/Rect.hpp
        src/FileIo/Model.hpp
        src/FileIo/Model.cpp
        src/Graphics/Model.hpp
        src/Asset/Handle.hpp
        src/Graphics/Framebuffer.cpp
        src/Graphics/Framebuffer.hpp
        src/Graphics/TextureArray.cpp
        src/Graphics/TextureArray.hpp
        src/Event/EventHandler.hpp
        src/Event/EventHandler.cpp
        src/App.cpp
        src/App.hpp
        src/Graphics/ModelRenderer.hpp
        src/Math/Transform.hpp
        src/Input/Input.hpp
        src/Input/InputHandler.cpp
        src/Input/InputHandler.hpp
        src/Ecs/World.cpp
        src/Ecs/World.hpp
)

add_library(${PROJECT_NAME} SHARED ${PROJECT_SOURCES})

target_precompile_headers(${PROJECT_NAME} PUBLIC Pch.hpp)
target_include_directories(${PROJECT_NAME} PUBLIC include src PRIVATE vendor)
target_compile_definitions(${PROJECT_NAME} PRIVATE FLK_DYLIB_BUILD)

target_link_libraries(${PROJECT_NAME} PUBLIC
        OpenGL::GL
        glfw
        glad
        SLog
        stbi
        assimp
)

if (UNIX AND NOT APPLE)
    find_package(ZLIB REQUIRED)
    target_link_libraries(FlockCore PRIVATE ZLIB::ZLIB)
endif ()

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