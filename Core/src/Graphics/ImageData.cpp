#include "ImageData.hpp"

namespace Pixf::Core::Graphics {
    ImageData::ImageData(const int width, const int height, const int channels, const Memory::Buffer &pixels) :
        m_Pixels(pixels), m_Width(width), m_Height(height), m_Channels(channels) {}

    int ImageData::GetWidth() const { return m_Width; }

    int ImageData::GetHeight() const { return m_Height; }

    int ImageData::GetChannels() const { return m_Channels; }

    const Memory::Buffer &ImageData::GetPixels() const { return m_Pixels; }
} // namespace Pixf::Core::Graphics
