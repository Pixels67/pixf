#include "Error/Error.hpp"
#include "Graphics/Gl/Shader.hpp"
#include "Graphics/Gl/Window.hpp"
#include "Graphics/Mesh.hpp"
#include "Pixf.hpp"

static const auto s_VertShader = R"(
#version 330 core

layout(location = 0) in vec3 aVertPos;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aTexCoord;

out vec2 vTexCoords;

void main() {
    gl_Position = vec4(aVertPos, 1.0);
    vTexCoords = aTexCoord;
}
)";

static const auto s_FragShader = R"(
#version 330 core

in vec2 vTexCoords;

out vec4 fragColor;

void main() {
    fragColor = vec4(vTexCoords, 0.0, 1.0);
}
)";

int main() {
    using namespace Pixf::Core;
    using namespace Pixf::Core::Math;
    using namespace Pixf::Core::Graphics;
    using namespace Pixf::Core::Debug;

    std::set_terminate(Error::HandleTerminate);

    Logger::Configure("Core", {.logFormat = "{m}"});

    const Gl::Window window = Gl::Window::Create({.title = "Pixf"});
    window.MakeCurrent();

    Event::EventManager eventManager;

    window.SetupEvents(eventManager);

    PIXF_GL_CALL(glViewport(0, 0, 800, 600));

    const Gl::Shader shader = Gl::Shader::Create(s_VertShader, s_FragShader);

    const std::vector<Vertex> vertices{
            {Vector3f(-1.0F, -1.0F, 0.0F), Vector3f(0.0F, 0.0F, 0.0F), Vector2f(0.0F, 0.0F)},
            {Vector3f(1.0F, -1.0F, 0.0F), Vector3f(0.0F, 0.0F, 0.0F), Vector2f(1.0F, 0.0F)},
            {Vector3f(-1.0F, 1.0F, 0.0F), Vector3f(0.0F, 0.0F, 0.0F), Vector2f(0.0F, 1.0F)},
            {Vector3f(1.0F, 1.0F, 0.0F), Vector3f(0.0F, 0.0F, 0.0F), Vector2f(1.0F, 1.0F)},
    };

    const std::vector<unsigned int> indices{0, 1, 2, 1, 2, 3};

    const Mesh mesh({vertices, indices});

    while (!window.ShouldClose()) {
        Gl::Window::PollEvents();

        shader.Bind();
        mesh.Bind();

        PIXF_GL_CALL(glClearColor(0.12F, 0.12F, 0.12F, 1.0F));
        PIXF_GL_CALL(glClear(GL_COLOR_BUFFER_BIT));

        PIXF_GL_CALL(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr));

        Gl::Shader::Unbind();
        Mesh::Unbind();

        window.SwapBuffers();
    }

    return 0;
}
