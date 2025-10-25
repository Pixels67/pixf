#include "RenderQueue.hpp"

#include <queue>

#include "RenderCommand.hpp"

namespace Pixf::Core::Graphics {
    void RenderQueue::Push(const RenderCommand &renderCommand, const RenderType type) {
        m_Queues[type].push(renderCommand);
    }

    RenderCommand RenderQueue::Pop(const RenderType type) {
        const RenderCommand cmd = m_Queues[type].front();
        m_Queues[type].pop();

        return cmd;
    }

    size_t RenderQueue::GetSize(const RenderType type) { return m_Queues[type].size(); }
    bool RenderQueue::IsEmpty(const RenderType type) { return m_Queues[type].empty(); }
} // namespace Pixf::Core::Graphics
