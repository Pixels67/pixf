#ifndef FLK_FILE_MODEL_HPP
#define FLK_FILE_MODEL_HPP

#include "Graphics/Mesh.hpp"
#include "Math/Color.hpp"

namespace Flock::FileIo {
    struct FLK_API MeshData {
        Graphics::MeshData data          = {};
        usize              materialIndex = 0;
    };

    struct FLK_API MaterialData {
        Color4u8 color     = Color4u8::White();
        f32      metallic  = 0.5F;
        f32      roughness = 0.5F;

        std::optional<std::string> colorMapPath     = std::nullopt;
        std::optional<std::string> metallicMapPath  = std::nullopt;
        std::optional<std::string> roughnessMapPath = std::nullopt;
    };

    FLK_API std::vector<MeshData>     LoadModelMeshes(const std::filesystem::path &filePath);
    FLK_API std::vector<MaterialData> LoadModelMaterials(const std::filesystem::path &filePath);
}

#endif //FLK_MODEL_HPP
