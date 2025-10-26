cmake_minimum_required(VERSION 3.5)
project(pixf_core)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/out/lib/${CMAKE_SYSTEM_NAME}-${CMAKE_SYSTEM_PROCESSOR}-${CMAKE_BUILD_TYPE})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/out/bin/${CMAKE_SYSTEM_NAME}-${CMAKE_SYSTEM_PROCESSOR}-${CMAKE_BUILD_TYPE})

set(VENDOR_DIR ${PROJECT_SOURCE_DIR}/vendor)

include(FetchContent)
set(FETCHCONTENT_QUIET FALSE)

set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

FetchContent_Declare(
        glfw
        GIT_REPOSITORY "https://github.com/glfw/glfw.git"
        GIT_TAG "3.4"
        GIT_PROGRESS TRUE
)

FetchContent_MakeAvailable(glfw)

FetchContent_Declare(
        glm
        GIT_REPOSITORY "https://github.com/g-truc/glm.git"
        GIT_TAG "1.0.1"
        GIT_PROGRESS TRUE
)

FetchContent_MakeAvailable(glm)

FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)

set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

FetchContent_Declare(
        assimp
        GIT_REPOSITORY "https://github.com/assimp/assimp.git"
        GIT_TAG "v6.0.2"
        GIT_PROGRESS TRUE
)

set(ASSIMP_BUILD_ALL_IMPORTERS_BY_DEFAULT OFF CACHE BOOL "" FORCE)
set(ASSIMP_BUILD_ALL_EXPORTERS_BY_DEFAULT OFF CACHE BOOL "" FORCE)

set(ASSIMP_BUILD_FBX_IMPORTER ON CACHE BOOL "" FORCE)
set(ASSIMP_BUILD_GLTF_IMPORTER ON CACHE BOOL "" FORCE)
set(ASSIMP_BUILD_OBJ_IMPORTER ON CACHE BOOL "" FORCE)

set(ASSIMP_BUILD_TESTS OFF CACHE BOOL "" FORCE)

FetchContent_MakeAvailable(assimp)

FetchContent_Declare(
        miniaudio
        GIT_REPOSITORY "https://github.com/mackron/miniaudio.git"
        GIT_TAG "0.11.23"
        GIT_PROGRESS TRUE
)

FetchContent_MakeAvailable(miniaudio)

FetchContent_Declare(
        imgui
        GIT_REPOSITORY "https://github.com/ocornut/imgui.git"
        GIT_TAG "v1.92.4"
        GIT_PROGRESS TRUE
)

FetchContent_MakeAvailable(imgui)

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

find_package(OpenGL REQUIRED)

set(
        PROJECT_SOURCES
        include/Pixf.hpp
        vendor/glad/include/glad/glad.h
        vendor/glad/src/glad.c
        vendor/stbi/stb_image.h
        vendor/stbi/stb_image.c
        src/Common.hpp
        src/Error/Result.hpp
        src/Graphics/Gl/Gl.hpp
        src/Graphics/Gl/Window.hpp
        src/Graphics/Gl/Window.cpp
        src/Graphics/Gl/Shader.cpp
        src/Graphics/Gl/Shader.hpp
        src/Graphics/Mesh.cpp
        src/Graphics/Mesh.hpp
        src/Graphics/Gl/VertexLayout.cpp
        src/Graphics/Gl/VertexLayout.hpp
        src/Graphics/Gl/VertexArray.cpp
        src/Graphics/Gl/VertexArray.hpp
        src/Graphics/Gl/VertexBuffer.cpp
        src/Graphics/Gl/VertexBuffer.hpp
        src/Graphics/Vertex.cpp
        src/Graphics/Vertex.hpp
        src/Graphics/Gl/IndexBuffer.cpp
        src/Graphics/Gl/IndexBuffer.hpp
        src/Entities/EntityManager.cpp
        src/Entities/EntityManager.hpp
        src/Entities/ComponentRegistry.hpp
        src/Entities/ComponentManager.hpp
        src/Entities/ComponentManager.cpp
        src/Entities/SystemsManager.hpp
        src/Entities/SystemsManager.cpp
        src/Entities/World.cpp
        src/Entities/World.hpp
        src/Time/Clock.cpp
        src/Time/Clock.hpp
        src/Graphics/Renderer.cpp
        src/Graphics/Renderer.hpp
        src/Graphics/ResourceManager.cpp
        src/Graphics/ResourceManager.hpp
        src/Graphics/Gl/Texture2D.cpp
        src/Graphics/Gl/Texture2D.hpp
        src/Entities/Components/Transform.cpp
        src/Entities/Components/Transform.hpp
        src/Entities/Components/RigidTransform.cpp
        src/Entities/Components/RigidTransform.hpp
        src/Entities/Components/Graphics/Camera.cpp
        src/Entities/Components/Graphics/Camera.hpp
        src/Graphics/RenderCommand.hpp
        src/Graphics/RenderQueue.cpp
        src/Graphics/RenderQueue.hpp
        src/Graphics/Material.cpp
        src/Graphics/Material.hpp
        src/Entities/Components/Graphics/DirectionalLight.hpp
        src/Entities/Components/Graphics/AmbientLight.hpp
        src/Entities/Components/Graphics/PointLight.hpp
        src/Graphics/Model.cpp
        src/Graphics/Model.hpp
        src/Event/Event.hpp
        src/Input/InputManager.cpp
        src/Input/InputManager.hpp
        src/Application.cpp
        src/Application.hpp
        src/WorldManager.cpp
        src/WorldManager.hpp
        src/Entities/Components/Graphics/ModelRenderer.hpp
        src/Audio/AudioManager.cpp
        src/Audio/AudioManager.hpp
        src/Entities/Components/Audio/AudioListener.hpp
        src/Entities/Components/Audio/AudioSource.hpp
        src/Debug/Logger.cpp
        src/Debug/Logger.hpp
        src/Time/LocalTime.cpp
        src/Time/LocalTime.hpp
        src/Gui/Gui.cpp
        src/Gui/Gui.hpp
)

add_library(${PROJECT_NAME} STATIC ${PROJECT_SOURCES})
target_include_directories(${PROJECT_NAME}
        PUBLIC
        include
        src
        ${VENDOR_DIR}/glad/include
        PRIVATE
        ${VENDOR_DIR}/stbi)

target_compile_definitions(${PROJECT_NAME} PRIVATE
        GLM_FORCE_LEFT_HANDED
)

target_link_libraries(${PROJECT_NAME} PUBLIC
        OpenGL::GL
        glfw
        glm
        assimp
        miniaudio
        imgui
)

# Tests
enable_testing()

add_executable(${PROJECT_NAME}_tests
        tests/EntityManagerTests.cpp
)

target_link_libraries(${PROJECT_NAME}_tests
        ${PROJECT_NAME}
        GTest::gtest_main
)

target_include_directories(${PROJECT_NAME}_tests
        PUBLIC include
        PRIVATE
        src
        ${VENDOR_DIR}/glad/include
        ${VENDOR_DIR}/stbi)

include(GoogleTest)
gtest_discover_tests(${PROJECT_NAME}_tests)

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