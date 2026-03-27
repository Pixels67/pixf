#ifndef FLK_SKYBOX_HPP
#define FLK_SKYBOX_HPP

#include <filesystem>

#include "Common.hpp"
#include "Texture.hpp"

namespace Flock::Graphics {
    struct FLK_API Skybox {
        std::filesystem::path filePath      = "";
        TextureConfig         textureConfig = {};
    };
}

#endif //FLK_SKYBOX_HPP
