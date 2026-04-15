#ifndef FLK_SKYBOX_HPP
#define FLK_SKYBOX_HPP

#include "Common.hpp"
#include "Serial/Archive.hpp"

namespace Flock::Graphics {
    struct FLK_API Skybox {
        std::string filePath = "";
    };

    FLK_ARCHIVE(Skybox, filePath)
}

#endif //FLK_SKYBOX_HPP
