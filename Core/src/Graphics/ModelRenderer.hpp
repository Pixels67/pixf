#ifndef FLK_MODELRENDERER_HPP
#define FLK_MODELRENDERER_HPP

#include "Common.hpp"
#include "Serial/Archive.hpp"

namespace Flock::Graphics {
    struct FLK_API ModelRenderer {
        std::string modelPath;
    };

    FLK_ARCHIVE(ModelRenderer, modelPath)
}

#endif //FLK_MODELRENDERER_HPP
