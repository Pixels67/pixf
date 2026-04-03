#ifndef FLK_FILE_MODEL_HPP
#define FLK_FILE_MODEL_HPP

#include "Graphics/Mesh.hpp"
#include "Graphics/Material.hpp"

namespace Flock::FileIo {
    struct FLK_API MeshData {
        Graphics::MeshData data          = {};
        usize              materialIndex = 0;
    };

    FLK_API std::vector<MeshData>           ReadModelMeshes(const std::filesystem::path &filePath);
    FLK_API std::vector<Graphics::Material> ReadModelMaterials(const std::filesystem::path &filePath);
}

#endif //FLK_MODEL_HPP
