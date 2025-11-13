#ifndef RENDERQUEUE_HPP
#define RENDERQUEUE_HPP

#include <queue>

#include "RenderCommand.hpp"

namespace Pixf::Core::Graphics {
    enum class RenderType : uint8_t {
        Opaque,
        Transparent,
    };

    class PIXF_API RenderQueue {
    public:
        RenderQueue() = default;

        RenderQueue(const RenderQueue &) = default;
        RenderQueue(RenderQueue &&) = delete;
        RenderQueue &operator=(const RenderQueue &) = default;
        RenderQueue &operator=(RenderQueue &&) = delete;

        ~RenderQueue() = default;

        void Push(const RenderCommand &renderCommand, RenderType type);
        RenderCommand Pop(RenderType type);

        size_t GetSize(RenderType type);
        bool IsEmpty(RenderType type);

    private:
        std::unordered_map<RenderType, std::queue<RenderCommand>> m_Queues;
    };
} // namespace Pixf::Core::Graphics

#endif //RENDERQUEUE_HPP
