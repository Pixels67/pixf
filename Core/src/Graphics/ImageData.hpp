#ifndef PIXF_IMAGEDATA_HPP
#define PIXF_IMAGEDATA_HPP

#include "Common.hpp"

namespace Pixf::Core::Graphics {
    class PIXF_API ImageData {
    public:
        ImageData(int width, int height, int channels, uint8_t *pixels);

        ImageData(const ImageData &) = delete;
        ImageData(ImageData &&other) = delete;
        ImageData &operator=(const ImageData &) = delete;
        ImageData &operator=(ImageData &&other) = delete;

        ~ImageData();

        int GetWidth() const;
        int GetHeight() const;
        int GetChannels() const;
        uint8_t *GetPixels() const;

    private:
        uint8_t *m_Pixels; // Hey, that's me :D
        int m_Width;
        int m_Height;
        int m_Channels;
    };
} // namespace Pixf::Core::File

#endif // PIXF_IMAGEDATA_HPP
