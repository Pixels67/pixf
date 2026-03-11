#ifndef FLK_MODEL_HPP
#define FLK_MODEL_HPP

#include "Material.hpp"
#include "Mesh.hpp"

namespace Flock::Graphics {
    struct RenderObject {
        Mesh     mesh;
        Material material;
    };

    struct Model {
        std::vector<RenderObject> objects;
    };
}

#endif //FLK_MODEL_HPP
