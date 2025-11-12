project(PixfCore)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/out/lib/${CMAKE_SYSTEM_NAME}-${CMAKE_SYSTEM_PROCESSOR}-${CMAKE_BUILD_TYPE})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/out/lib/${CMAKE_SYSTEM_NAME}-${CMAKE_SYSTEM_PROCESSOR}-${CMAKE_BUILD_TYPE})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/out/bin/${CMAKE_SYSTEM_NAME}-${CMAKE_SYSTEM_PROCESSOR}-${CMAKE_BUILD_TYPE})

set(CMAKE_POSITION_INDEPENDENT_CODE ON)

set(VENDOR_DIR ${PROJECT_SOURCE_DIR}/vendor)

# Uncomment when debugging memory issues
#
#if (CMAKE_BUILD_TYPE STREQUAL "Debug" AND NOT MSVC)
#    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address -g")
#    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fsanitize=address -g")
#endif()

find_package(OpenGL REQUIRED)

set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
CPMAddPackage(
        NAME googletest
        URL "https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip"
)

CPMAddPackage(
        URI "gh:glfw/glfw#3.4"
        OPTIONS
        "GLFW_BUILD_DOCS OFF"
        "GLFW_BUILD_TESTS OFF"
        "GLFW_BUILD_EXAMPLES OFF"
)

CPMAddPackage("gh:g-truc/glm#1.0.1")

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

CPMAddPackage("gh:mackron/miniaudio#0.11.23")

# Boost libs (Boost::uuid and Boost::json + their deps)
CPMAddPackage("gh:boostorg/static_assert#boost-1.89.0")
CPMAddPackage("gh:boostorg/core#boost-1.89.0")
CPMAddPackage("gh:boostorg/assert#boost-1.89.0")
CPMAddPackage("gh:boostorg/config#boost-1.89.0")
CPMAddPackage("gh:boostorg/throw_exception#boost-1.89.0")
CPMAddPackage("gh:boostorg/mp11#boost-1.89.0")
CPMAddPackage("gh:boostorg/describe#boost-1.89.0")
CPMAddPackage("gh:boostorg/container_hash#boost-1.89.0")
CPMAddPackage("gh:boostorg/type_traits#boost-1.89.0")
CPMAddPackage("gh:boostorg/uuid#boost-1.89.0")
CPMAddPackage("gh:boostorg/align#boost-1.89.0")
CPMAddPackage("gh:boostorg/variant2#boost-1.89.0")
CPMAddPackage("gh:boostorg/predef#boost-1.89.0")
CPMAddPackage("gh:boostorg/winapi#boost-1.89.0")
CPMAddPackage("gh:boostorg/system#boost-1.89.0")
CPMAddPackage("gh:boostorg/move#boost-1.89.0")
CPMAddPackage("gh:boostorg/intrusive#boost-1.89.0")
CPMAddPackage("gh:boostorg/container#boost-1.89.0")
CPMAddPackage("gh:boostorg/endian#boost-1.89.0")
CPMAddPackage("gh:boostorg/json#boost-1.89.0")

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
        include/Pixf.hpp
        vendor/glad/include/glad/glad.h
        vendor/glad/src/glad.c
        vendor/stbi/stb_image.h
        vendor/stbi/stb_image.c
        src/Common.hpp
        src/Error/Result.hpp
        src/Graphics/Gl/Gl.hpp
        src/Window.hpp
        src/Window.cpp
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
        src/Assets/AssetManager.cpp
        src/Assets/AssetManager.hpp
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
        src/Audio/AudioEngine.cpp
        src/Audio/AudioEngine.hpp
        src/Entities/Components/Audio/AudioListener.hpp
        src/Entities/Components/Audio/AudioSource.hpp
        src/Debug/Logger.cpp
        src/Debug/Logger.hpp
        src/Time/LocalTime.cpp
        src/Time/LocalTime.hpp
        src/Gui/Gui.cpp
        src/Gui/Gui.hpp
        src/File/File.hpp
        src/File/File.cpp
        src/Json/Json.hpp
        src/Serialization/Serialization.hpp
        src/Serialization/Serializable.hpp
        src/Entities/Blueprint.cpp
        src/Entities/Blueprint.hpp
        src/Entities/ComponentRegistry.cpp
)

add_library(${PROJECT_NAME} SHARED ${PROJECT_SOURCES})

target_include_directories(${PROJECT_NAME} PUBLIC include src ${VENDOR_DIR}/glad/include)
target_include_directories(${PROJECT_NAME} PRIVATE ${VENDOR_DIR}/stbi)

target_compile_definitions(${PROJECT_NAME} PRIVATE
        GLM_FORCE_LEFT_HANDED
        PIXF_DYLIB_BUILD
)

target_link_libraries(${PROJECT_NAME} PUBLIC
        OpenGL::GL
        glfw
        glm::glm
        assimp::assimp
        miniaudio
        imgui
        Boost::uuid
        Boost::json
)

# Tests
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

add_executable(${PROJECT_NAME}Tests
        tests/EntityManagerTests.cpp
)

target_link_libraries(${PROJECT_NAME}Tests
        ${PROJECT_NAME}
        GTest::gtest_main
)

target_include_directories(${PROJECT_NAME}Tests
        PUBLIC include
        PRIVATE
        src
        ${VENDOR_DIR}/glad/include
        ${VENDOR_DIR}/stbi
)

if(WIN32)
    add_custom_command(TARGET ${PROJECT_NAME}Tests POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
            $<TARGET_FILE:${PROJECT_NAME}>
            $<TARGET_FILE_DIR:${PROJECT_NAME}Tests>
            COMMENT "Copying PixfCore DLL to test directory"
    )
endif()

include(GoogleTest)
gtest_discover_tests(${PROJECT_NAME}Tests DISCOVERY_MODE PRE_TEST)

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

# Compiler options
if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    if (MSVC)
        target_compile_options(${PROJECT_NAME} PRIVATE
                /W4
                /Zi
                /RTC1
        )
    else ()
        target_compile_options(${PROJECT_NAME} PRIVATE
                -Wall
                -Wextra
                -Wpedantic
                -g3
                -fvisibility=hidden
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
                -fvisibility=hidden
        )
    endif ()
endif ()