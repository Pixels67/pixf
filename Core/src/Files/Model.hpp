#ifndef PIXF_FILES_MODEL_HPP
#define PIXF_FILES_MODEL_HPP

#include <assimp/scene.h>

#include "Graphics/Model.hpp"
#include "Resources.hpp"
#include "Math/Color.hpp"

namespace Pixf::Core::Files {
    struct MeshData {
        Graphics::MeshData data;
        size_t materialIndex;
    };

    struct MaterialData {
        std::optional<std::string> diffusePath = std::nullopt;
        std::optional<std::string> specularPath = std::nullopt;
        Math::Color3u8 diffuseColor;
        Math::Color3u8 specularColor;
    };

    std::vector<MeshData> LoadModelMeshes(const std::string &filepath);
    std::vector<MaterialData> LoadModelMaterials(const std::string &filepath);
} // namespace Pixf::Core::Files

#endif // PIXF_FILES_MODEL_HPP
