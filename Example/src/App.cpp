#include "Files/Image.hpp"
#include "Graphics/Handle.hpp"
#include "Graphics/Material.hpp"
#include "Graphics/MeshStore.hpp"
#include "Graphics/TextureStore.hpp"
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

uniform sampler2D uTex;

void main() {
    fragColor = texture(uTex, vTexCoords);
}
)";

int main() {
    using namespace Pixf::Core;
    using namespace Pixf::Core::Math;
    using namespace Pixf::Core::Graphics;
    using namespace Pixf::Core::Debug;

    std::set_terminate(Error::HandleTerminate);

    const Gl::Window window = Gl::Window::Create({.title = "Pixf"});
    window.MakeCurrent();

    Event::EventManager eventManager;
    window.SetupEvents(eventManager);

    PIXF_GL_CALL(glViewport(0, 0, 800, 600));

    ShaderStore shaderStore;
    const ShaderHandle shader = shaderStore.Load(s_VertShader, s_FragShader);

    TextureStore textureStore;
    Texture2DHandle texture;
    {
        const ImageData image = Files::LoadImage("img.png");
        texture = textureStore.Load(image, {});
    }

    Material mat;
    mat.SetTexture2D("uTex", texture);

    const std::vector<Vertex> vertices{
            {Vector3f(-1.0F, -1.0F, 0.0F), Vector3f(0.0F, 0.0F, 0.0F), Vector2f(0.0F, 0.0F)},
            {Vector3f(1.0F, -1.0F, 0.0F), Vector3f(0.0F, 0.0F, 0.0F), Vector2f(1.0F, 0.0F)},
            {Vector3f(-1.0F, 1.0F, 0.0F), Vector3f(0.0F, 0.0F, 0.0F), Vector2f(0.0F, 1.0F)},
            {Vector3f(1.0F, 1.0F, 0.0F), Vector3f(0.0F, 0.0F, 0.0F), Vector2f(1.0F, 1.0F)},
    };

    const std::vector<unsigned int> indices{0, 1, 2, 1, 2, 3};

    MeshStore meshStore;
    const MeshHandle mesh = meshStore.Load({vertices, indices});

    while (!window.ShouldClose()) {
        Gl::Window::PollEvents();

        shaderStore.Get(shader).Bind();
        meshStore.Get(mesh).Bind();
        textureStore.Get(mat.GetTexture2D("uTex").value()).Bind(0);

        PIXF_GL_CALL(glClearColor(0.12F, 0.12F, 0.12F, 1.0F));
        PIXF_GL_CALL(glClear(GL_COLOR_BUFFER_BIT));

        PIXF_GL_CALL(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr));

        Gl::Texture2D::Unbind(0);
        Gl::Shader::Unbind();
        Mesh::Unbind();

        window.SwapBuffers();
    }

    return 0;
}
