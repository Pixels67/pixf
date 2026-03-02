#ifndef FLK_MODEL_HPP
#define FLK_MODEL_HPP

#include "Material.hpp"
#include "Mesh.hpp"

namespace Flock::Graphics {
    struct Model {
        std::vector<Mesh>     meshes;
        std::vector<Material> materials;
    };
}

#endif //FLK_MODEL_HPP
