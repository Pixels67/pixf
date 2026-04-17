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

        Model &Add(Mesh &&mesh, const Material &material) {
            objects.push_back(RenderObject{.mesh = std::move(mesh), .material = material});
            return *this;
        }
    };
}

#endif //FLK_MODEL_HPP
