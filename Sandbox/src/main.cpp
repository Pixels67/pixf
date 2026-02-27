#include "Flock.hpp"
#include "FileIo/Image.hpp"
#include "Glfw/Window.hpp"
#include "Graphics/Gl.hpp"

using namespace Flock;
using namespace Flock::Graphics;

static const auto s_VertShader = R"(
#version 330 core

layout(location = 0) in vec3 aVertPos;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aTexCoords;

out vec2 vTexCoords;
out vec3 vColor;

void main() {
    gl_Position = vec4(aVertPos, 1.0);
    vTexCoords = aTexCoords;
    vColor = aNormal;
}
)";

static const auto s_FragShader = R"(
#version 330 core

in vec2 vTexCoords;
in vec3 vColor;

out vec4 fragColor;

uniform sampler2D uTex;

void main() {
    fragColor = vec4(vec3(texture(uTex, vTexCoords)) * vColor, 1.0);
}
)";

static const std::vector s_Vertices = {
    Vertex{{-1.0F, -1.0F, 0.0F}, {0.0F, 0.0F, 1.0F}, {0.0F, 0.0F}},
    Vertex{{-1.0F, 1.0F, 0.0F}, {1.0F, 0.0F, 0.0F}, {0.0F, 1.0F}},
    Vertex{{1.0F, -1.0F, 0.0F}, {0.0F, 0.0F, 1.0F}, {1.0F, 0.0F}},
    Vertex{{1.0F, 1.0F, 0.0F}, {1.0F, 0.0F, 0.0F}, {1.0F, 1.0F}}
};

static const std::vector s_Indices = {
    0u, 1u, 2u,
    2u, 3u, 1u
};

int main() {
    const Glfw::Window window = Glfw::Window::Create().value();
    window.MakeCurrent();

    const Shader   vertShader = Shader::Create(VertexShader, s_VertShader).value();
    const Shader   fragShader = Shader::Create(FragmentShader, s_FragShader).value();
    const Pipeline pipeline   = Pipeline::Create(vertShader, fragShader).value();

    if (!pipeline.Bind()) {
        return -1;
    }

    Texture2D texture = Texture2D::Create(
        FileIo::ReadImage("Checkerboard.png"),
        {.filterMode = Nearest}
    );

    if (!pipeline.SetUniform("uTex", texture)) {
        return -1;
    }

    const Mesh mesh = Mesh::Create({
        .vertices = s_Vertices,
        .indices  = s_Indices
    }).value();

    if (!mesh.Bind()) {
        return -1;
    }

    while (!window.ShouldClose()) {
        Glfw::PollEvents();

        glViewport(0, 0, window.GetSize().x, window.GetSize().y);
        glClearColor(0.15F, 0.2F, 0.2F, 1.0F);
        glClear(GL_COLOR_BUFFER_BIT);

        glDrawElements(GL_TRIANGLES, mesh.GetIndexCount(), GL_UNSIGNED_INT, nullptr);

        window.SwapBuffers();
    }

    Mesh::Unbind();
    Pipeline::Unbind();
}
