#ifndef FLK_ASSETS_HPP
#define FLK_ASSETS_HPP

#include "Common.hpp"
#include "AssetLoader.hpp"

namespace Flock::Asset {
    struct FLK_API Assets {
        AssetLoader &loader;

        template<typename T>
        T &Get(const std::filesystem::path &filePath) const {
            return loader.Get<T>(filePath).value();
        }

        template<typename T>
        bool Load(const std::filesystem::path &filePath) const {
            return loader.Load<T>(filePath);
        }

        bool SetDefaultPipeline(const PipelineType type, const std::filesystem::path &filePath) const {
            return loader.SetDefaultPipeline(type, filePath);
        }
    };
}

#endif //FLK_ASSETS_HPP
