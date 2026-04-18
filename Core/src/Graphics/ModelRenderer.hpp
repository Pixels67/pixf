#ifndef FLK_MODELRENDERER_HPP
#define FLK_MODELRENDERER_HPP

#include "Common.hpp"
#include "Serial/Archive.hpp"

namespace Flock::Graphics {
    struct Model;

    struct FLK_API ModelRenderer {
        Asset::AssetHandle<Model> model;
    };

    FLK_ARCHIVE(ModelRenderer, model)
}

#endif //FLK_MODELRENDERER_HPP
