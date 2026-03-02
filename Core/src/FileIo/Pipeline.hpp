#ifndef FLK_FILEIO_PIPELINE_HPP
#define FLK_FILEIO_PIPELINE_HPP

#include "Graphics/Pipeline.hpp"

namespace Flock::FileIo {
    std::optional<Graphics::Pipeline> FLK_API ReadPipeline(const std::filesystem::path &filePath);
}

#endif //FLK_FILEIO_PIPELINE_HPP