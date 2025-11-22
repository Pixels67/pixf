project(PixfCore)

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
        NAME fmt
        GITHUB_REPOSITORY "fmtlib/fmt"
        GIT_TAG 12.1.0
        "FMT_DOC OFF"
        "FMT_TEST OFF"
        "FMT_INSTALL OFF"
        "FMT_HEADER_ONLY ON"
)

if (WIN32)
    set(ASSIMP_BUILD_ZLIB ON CACHE BOOL "" FORCE)
else ()
    set(ASSIMP_BUILD_ZLIB OFF CACHE BOOL "" FORCE)
    set(ASSIMP_INSTALL OFF)
    set(ASSIMP_INSTALL_PDB OFF)
    find_package(ZLIB REQUIRED)
endif ()

CPMAddPackage(
        URI "gh:assimp/assimp@6.0.2"
        OPTIONS
        "ASSIMP_BUILD_ALL_IMPORTERS_BY_DEFAULT OFF"
        "ASSIMP_BUILD_ALL_EXPORTERS_BY_DEFAULT OFF"
        "ASSIMP_BUILD_FBX_IMPORTER ON"
        "ASSIMP_BUILD_GLTF_IMPORTER ON"
        "ASSIMP_BUILD_OBJ_IMPORTER ON"
        "ASSIMP_BUILD_TESTS OFF"
)

CPMAddPackage(
        URI "gh:skypjack/entt#v3.16.0"
        OPTIONS
        "ENTT_BUILD_TESTING OFF"
        "ENTT_BUILD_TESTBED OFF"
        "ENTT_BUILD_DOCS OFF"
)

CPMAddPackage(
        URI "gh:mariusbancila/stduuid#v1.2.3"
        OPTIONS
        "UUID_BUILD_TESTS OFF"
)

CPMAddPackage(
        URI "gh:USCiLab/cereal#v1.3.2"
        OPTIONS
        "BUILD_DOC OFF"
        "BUILD_TESTS OFF"
        "BUILD_SANDBOX OFF"
        "SKIP_PERFORMANCE_COMPARISON ON"
)

add_library(stbi STATIC vendor/stbi/stbi.h vendor/stbi/stbi.c)

CPMAddPackage(
        NAME imgui
        GITHUB_REPOSITORY "ocornut/imgui"
        GIT_TAG "v1.92.4"
)

add_library(imgui STATIC
        ${imgui_SOURCE_DIR}/imgui.cpp
        ${imgui_SOURCE_DIR}/imgui_demo.cpp
        ${imgui_SOURCE_DIR}/imgui_draw.cpp
        ${imgui_SOURCE_DIR}/imgui_tables.cpp
        ${imgui_SOURCE_DIR}/imgui_widgets.cpp
        ${imgui_SOURCE_DIR}/backends/imgui_impl_glfw.cpp
        ${imgui_SOURCE_DIR}/backends/imgui_impl_opengl3.cpp
)

target_include_directories(imgui PUBLIC
        ${imgui_SOURCE_DIR}
        ${imgui_SOURCE_DIR}/backends
)

target_link_libraries(imgui PUBLIC glfw OpenGL::GL)

set(PROJECT_SOURCES
        Pch.hpp
        include/Pixf.hpp
        src/Common.hpp
        src/TypeId.hpp
        src/TypeId.cpp
        src/Event/EventManager.cpp
        src/Event/EventManager.hpp
        src/Debug/Logger.cpp
        src/Debug/Logger.hpp
        src/Graphics/Gl/Gl.hpp
        src/Graphics/Gl/Gl.cpp
        src/Graphics/Gl/Window.cpp
        src/Graphics/Gl/Window.hpp
        src/Math/Vector.hpp
        src/Error/Error.hpp
        src/Error/Error.cpp
        src/Graphics/Gl/Shader.cpp
        src/Graphics/Gl/Shader.hpp
        src/Math/Color.hpp
        src/Math/Math.hpp
        src/Math/Matrix.hpp
        src/Math/Quaternion.hpp
        src/Math/Math.cpp
        src/Graphics/Gl/VertexLayout.cpp
        src/Graphics/Gl/VertexLayout.hpp
        src/Graphics/Vertex.hpp
        src/Graphics/Vertex.cpp
        src/Graphics/Gl/Buffer.cpp
        src/Graphics/Gl/Buffer.hpp
        src/Memory/Buffer.cpp
        src/Memory/Buffer.hpp
        src/Graphics/Gl/VertexArray.cpp
        src/Graphics/Gl/VertexArray.hpp
        src/Graphics/Mesh.cpp
        src/Graphics/Mesh.hpp
        src/Graphics/Material.hpp
        src/Graphics/Handle.hpp
        src/Graphics/ShaderStore.cpp
        src/Graphics/ShaderStore.hpp
        src/Graphics/MeshStore.cpp
        src/Graphics/MeshStore.hpp
        src/Graphics/Gl/Texture2D.cpp
        src/Graphics/Gl/Texture2D.hpp
        src/Graphics/ImageData.cpp
        src/Graphics/ImageData.hpp
        src/Files/Image.cpp
        src/Files/Image.hpp
        src/Graphics/TextureStore.cpp
        src/Graphics/TextureStore.hpp
        src/Graphics/Material.cpp
        src/Graphics/MaterialStore.cpp
        src/Graphics/MaterialStore.hpp
        src/Graphics/Model.hpp
        src/Files/Model.cpp
        src/Files/Model.hpp
        src/Graphics/Resources.hpp
        src/Graphics/RenderCommand.hpp
        src/Graphics/Renderer.cpp
        src/Graphics/Renderer.hpp
        src/Graphics/RenderPass.hpp
        src/Application/Application.cpp
        src/Application/Application.hpp
        src/Time/Clock.cpp
        src/Time/Clock.hpp
        src/Graphics/Gl/Viewport.cpp
        src/Graphics/Gl/Viewport.hpp
        src/Application/Stage.hpp
        src/Event/Event.hpp
        src/Application/Pipeline.cpp
        src/Application/Pipeline.hpp
        src/Application/State.hpp
        src/Input/InputManager.hpp
        src/Input/InputManager.cpp
        src/Gui/Gui.hpp
        src/Gui/Gui.cpp
        src/Entities/Registry.hpp
        src/Entities/Registry.cpp
        src/Entities/SystemRegistry.cpp
        src/Entities/SystemRegistry.hpp
        src/Entities/System.hpp
        src/Entities/System.cpp
        src/Entities/Components/Transform.hpp
        src/Entities/Components/RigidTransform.hpp
        src/Files/Assets/AssetManager.cpp
        src/Files/Assets/AssetManager.hpp
        src/Uuid/Uuid.hpp
        src/Uuid/Uuid.cpp
        src/Files/File.hpp
        src/Files/File.cpp
        src/Entities/Components/Graphics/Model.hpp
        src/Serialization/JsonArchive.hpp
        src/Serialization/Serialization.hpp
)

add_library(${PROJECT_NAME} SHARED ${PROJECT_SOURCES})

target_precompile_headers(${PROJECT_NAME} PUBLIC Pch.hpp)
target_include_directories(${PROJECT_NAME} PUBLIC include src PRIVATE vendor)
target_compile_definitions(${PROJECT_NAME} PRIVATE PIXF_DYLIB_BUILD)

target_link_libraries(${PROJECT_NAME} PUBLIC
        OpenGL::GL
        glfw
        glad
        fmt
        assimp::assimp
        EnTT::EnTT
        stduuid
        cereal
        stbi
        imgui
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
endif ()

if (NOT MSVC)
    target_compile_options(${PROJECT_NAME} PRIVATE -fvisibility=hidden)
endif ()