#ifndef PIXF_LOADIMAGE_HPP
#define PIXF_LOADIMAGE_HPP

#include "Error/Error.hpp"
#include "Graphics/ImageData.hpp"

namespace Pixf::Core::Files {
    class PIXF_API ImageLoadError final : public Error::Error {
    public:
        explicit ImageLoadError(const std::string &path) : Error("Failed to load image: " + path) {}
    };

    PIXF_API Graphics::ImageData LoadImage(const std::string &path, bool flipVertically = true);
} // namespace Pixf::Core::Files

#endif // PIXF_LOADIMAGE_HPP
