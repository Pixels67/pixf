#ifndef FLK_SKYBOX_HPP
#define FLK_SKYBOX_HPP

#include "Common.hpp"

namespace Flock::Graphics {
    struct FLK_API Skybox {
        std::string filePath = "";
    };

    inline auto Reflect(Skybox &skybox) {
        return Reflectable{
            "Skybox",
            std::make_tuple(
                Field{"filePath", &skybox.filePath}
            )
        };
    }
}

#endif //FLK_SKYBOX_HPP
