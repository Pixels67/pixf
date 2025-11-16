#include "ImageData.hpp"

#include <stbi/stbi.h>

namespace Pixf::Core::Graphics {
    ImageData::ImageData(const int width, const int height, const int channels, uint8_t *pixels) :
        m_Pixels(pixels), m_Width(width), m_Height(height), m_Channels(channels) {}

    ImageData::~ImageData() {
        if (m_Pixels != nullptr) {
            stbi_image_free(m_Pixels);
        }
    }

    int ImageData::GetWidth() const { return m_Width; }

    int ImageData::GetHeight() const { return m_Height; }

    int ImageData::GetChannels() const { return m_Channels; }

    uint8_t *ImageData::GetPixels() const { return m_Pixels; }
} // namespace Pixf::Core::Graphics
