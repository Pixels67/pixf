#ifndef FLK_FILEIO_PIPELINE_HPP
#define FLK_FILEIO_PIPELINE_HPP

#include <filesystem>
#include <optional>

#include "Graphics/Pipeline.hpp"
#include "Common.hpp"

namespace Flock::FileIo {
    std::optional<Graphics::Pipeline> FLK_API ReadPipeline(const std::filesystem::path &filePath);
}

#endif //FLK_FILEIO_PIPELINE_HPP