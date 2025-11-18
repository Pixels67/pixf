#ifndef PIXF_IMAGEDATA_HPP
#define PIXF_IMAGEDATA_HPP

#include "Common.hpp"
#include "Memory/Buffer.hpp"

namespace Pixf::Core::Graphics {
    class PIXF_API ImageData {
    public:
        ImageData(int width, int height, int channels, const Memory::Buffer &pixels);

        int GetWidth() const;
        int GetHeight() const;
        int GetChannels() const;
        const Memory::Buffer &GetPixels() const;

    private:
        Memory::Buffer m_Pixels;
        int m_Width;
        int m_Height;
        int m_Channels;
    };
} // namespace Pixf::Core::File

#endif // PIXF_IMAGEDATA_HPP
