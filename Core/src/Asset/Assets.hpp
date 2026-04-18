#ifndef FLK_ASSETS_HPP
#define FLK_ASSETS_HPP

#include "Common.hpp"
#include "AssetLoader.hpp"

namespace Flock::Asset {
    struct FLK_API Assets {
        AssetLoader &loader;

        template<typename T>
        T *Get(const std::filesystem::path &filePath) const {
            return loader.Get<T>(filePath);
        }

        template<typename T>
        AssetHandle<T> Load(const std::filesystem::path &filePath) const {
            return loader.Load<T>(filePath);
        }

        template<typename T>
        AssetHandle<T> Register(T &&asset) const {
            return loader.Register<T>(std::forward<T>(asset));
        }

        bool SetPipeline(const std::string &name, const std::filesystem::path &filePath) const {
            return loader.SetPipeline(name, loader.Load<Graphics::Pipeline>(filePath));
        }
    };
}

#endif //FLK_ASSETS_HPP
