#ifndef FLK_FILE_MODEL_HPP
#define FLK_FILE_MODEL_HPP

#include "Graphics/Mesh.hpp"
#include "Graphics/Material.hpp"
#include "Math/Color.hpp"

namespace Flock::FileIo {
    struct FLK_API MeshData {
        Graphics::MeshData data          = {};
        usize              materialIndex = 0;
    };

    FLK_API std::vector<MeshData>           LoadModelMeshes(const std::filesystem::path &filePath);
    FLK_API std::vector<Graphics::Material> LoadModelMaterials(const std::filesystem::path &filePath);
}

#endif //FLK_MODEL_HPP
