#include "Asset/AssetLoader.hpp"
#include "Glfw/Window.hpp"
#include "Graphics/Renderer.hpp"

using namespace Flock;
using namespace Flock::Graphics;

int main() {
    const Glfw::Window window = Glfw::Window::Create({.size = {800, 800}}).value();
    window.MakeCurrent();

    Renderer           renderer;
    Asset::AssetLoader loader;

    const auto pipeline = loader.Load<Pipeline>("../../../assets/shader.glsl").value();
    loader.SetDefaultPipeline(Asset::PipelineType::Pbr, pipeline);

    const auto modelAsset = loader.Load<Model>("../../../assets/dragon1.ply").value();
    Model &    model      = loader.Get(modelAsset).value();

    float rot = 35.0F;
    while (!window.ShouldClose()) {
        Glfw::PollEvents();

        Vector3f   position = {0.0F, -0.6F, 1.2F};
        Quaternion rotation = Quaternion::Euler(0.0F, rot, 0.0F);

        std::vector<RenderObject> objects;
        objects.reserve(model.meshes.size());
        for (auto &mesh: model.meshes) {
            objects.push_back({
                .mesh     = mesh,
                .position = position,
                .rotation = rotation
            });
        }

        auto shadowMap = Renderer::GenerateShadowMap(
            std::move(objects),
            {2048, 2048},
            {0.5F, 1.0F, -0.5F},
            position,
            1.0F
        ).value();

        renderer.BeginPass({
            .projection       = Projection::Perspective,
            .viewport         = {{0, 0}, window.GetSize()},
            .ambientIntensity = 0.1F,
            .lights           = {
                {
                    .position  = {0.5F, 1.0F, -0.5F},
                    .color     = {240, 220, 160},
                    .intensity = 10.0F,
                    .shadowMap = std::ref(shadowMap),
                }
            }
        });

        for (usize i = 0; i < model.meshes.size(); i++) {
            renderer.Submit({
                .mesh     = model.meshes[i],
                .material = model.materials[i],
                .position = position,
                .rotation = rotation
            });
        }

        renderer.Render(loader);
        window.SwapBuffers();

        rot += 1.0F;
    }
}
