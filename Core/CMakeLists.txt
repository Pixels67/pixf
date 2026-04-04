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
        NAME json
        GITHUB_REPOSITORY nlohmann/json
        GIT_TAG v3.12.0
        GIT_SHALLOW TRUE
)

CPMAddPackage(
        NAME reactphysics3d
        GITHUB_REPOSITORY Pixels67/reactphysics3d
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

add_library(stbi STATIC vendor/stbi/stbi.h vendor/stbi/stbi.c)
add_library(stbv STATIC vendor/stbv/stbv.c)

add_library(nanovg STATIC
        vendor/nanovg/src/nanovg.h
        vendor/nanovg/src/nanovg.c
        vendor/nanovg/src/nanovg_gl.h
        vendor/nanovg/src/nanovg_gl_utils.h
        vendor/nanovg/src/fontstash.h
        vendor/nanovg/src/stb_truetype.h
)

target_compile_definitions(nanovg PRIVATE NVG_NO_STB)

set(SOLOUD_BACKEND_OGGVORBIS OFF)
file(GLOB_RECURSE SOLOUD_SOURCES CONFIGURE_DEPENDS "vendor/soloud/src/*.cpp" "vendor/soloud/src/*.h")
add_library(soloud STATIC ${SOLOUD_SOURCES})
target_include_directories(soloud PUBLIC vendor/soloud/include vendor/soloud/src)
target_compile_definitions(soloud PUBLIC WITH_MINIAUDIO)

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
        src/Graphics/Texture.cpp
        src/Graphics/Texture.hpp
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
        src/Time/Time.hpp
        src/Audio/AudioPlayer.cpp
        src/Audio/AudioPlayer.hpp
        src/Audio/AudioClip.hpp
        src/FileIo/Audio.hpp
        src/FileIo/Audio.cpp
        src/Asset/AssetLoader.cpp
        src/Audio/AudioSource.hpp
        src/Audio/AudioListener.hpp
        src/Reflect.hpp
        src/Serial/Json.cpp
        src/Serial/Json.hpp
        src/Serial/Archive.hpp
        src/Serial/JsonArchive.hpp
        src/Serial/JsonArchive.cpp
        src/Graphics/Camera.hpp
        src/Graphics/Light.hpp
        src/Physics/RigidBody.hpp
        src/Physics/Collider.hpp
        src/Math/RigidTransform.hpp
        src/Physics/PhysicsEngine.cpp
        src/Physics/PhysicsEngine.hpp
        src/Asset/Assets.hpp
        src/Graphics/CubeMap.cpp
        src/Graphics/CubeMap.hpp
        src/Graphics/Skybox.hpp
        src/Graphics/SpriteRenderer.hpp
        src/FileIo/World.hpp
        src/FileIo/World.cpp
        src/Gui/Text.hpp
        src/Gui/RectTransform.hpp
        src/Gui/GuiRenderer.cpp
        src/Gui/GuiRenderer.hpp
        src/Gui/Nvg.hpp
        src/Gui/Font.hpp
        src/FileIo/Font.hpp
        src/FileIo/Font.cpp
        src/Gui/Button.hpp
        src/Event/EventRegistry.cpp
        src/Event/EventRegistry.hpp
        src/Gui/Image.hpp
)

add_library(${PROJECT_NAME} SHARED ${PROJECT_SOURCES})

target_precompile_headers(${PROJECT_NAME} PUBLIC Pch.hpp)
target_include_directories(${PROJECT_NAME} PUBLIC include src PRIVATE vendor/stbi vendor/nanovg/src)
target_compile_definitions(${PROJECT_NAME} PRIVATE FLK_DYLIB_BUILD)

target_link_libraries(${PROJECT_NAME} PUBLIC
        OpenGL::GL
        glfw
        glad
        SLog
        stbi
        stbv
        soloud
        assimp
        nlohmann_json
        reactphysics3d
        nanovg
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