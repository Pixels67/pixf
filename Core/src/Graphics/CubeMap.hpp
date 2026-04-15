#ifndef FLK_CUBEMAP_HPP
#define FLK_CUBEMAP_HPP

#include <optional>

#include "Common.hpp"
#include "Texture.hpp"

namespace Flock::Graphics {
struct Image;
}

namespace Flock::Graphics {
    class FLK_API CubeMap {
        u32           m_Id     = 0;
        TextureConfig m_Config = {};

    public:
        static std::optional<CubeMap> FromImages(
            const Image & right,
            const Image & left,
            const Image & up,
            const Image & down,
            const Image & forward,
            const Image & back,
            TextureConfig config = {}
        );

        static CubeMap SingleColor(Color4u8 color, TextureConfig config = {});

        CubeMap() = default;
        ~CubeMap();

        CubeMap(const CubeMap &other) = delete;
        CubeMap(CubeMap &&other) noexcept;

        CubeMap &operator=(const CubeMap &other) = delete;
        CubeMap &operator=(CubeMap &&other) noexcept;

        void Clear() const;

        static void SetActiveUnit(u8 unit);

        bool Bind() const;

        static void Unbind();

        void Configure(TextureConfig config);
    };
}

#endif //FLK_CUBEMAP_HPP
