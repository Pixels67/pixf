#include "Buffer.hpp"

#include "../Graphics/Vertex.hpp"

namespace Pixf::Core::Memory {
    Buffer::Buffer(const size_t size) : m_Data(size > 0 ? malloc(size) : nullptr), m_Size(size) {}

    Buffer::Buffer(const void *src, const size_t size) : m_Data(size > 0 ? malloc(size) : nullptr), m_Size(size) {
        if (m_Data != nullptr && src != nullptr) {
            memcpy(m_Data, src, size);
        }
    }

    Buffer::~Buffer() { free(m_Data); }

    Buffer::Buffer(const Buffer &other) :
        m_Data(other.m_Size > 0 ? malloc(other.m_Size) : nullptr), m_Size(other.m_Size) {
        if (m_Data != nullptr && other.m_Data != nullptr) {
            memcpy(m_Data, other.m_Data, m_Size);
        }
    }

    Buffer::Buffer(Buffer &&other) noexcept : m_Data(other.m_Data), m_Size(other.m_Size) {
        other.m_Data = nullptr;
        other.m_Size = 0;
    }

    Buffer &Buffer::operator=(const Buffer &other) {
        if (this != &other) {
            free(m_Data);

            m_Size = other.m_Size;
            m_Data = m_Size > 0 ? malloc(m_Size) : nullptr;

            if (m_Data != nullptr && other.m_Data != nullptr) {
                memcpy(m_Data, other.m_Data, m_Size);
            }
        }

        return *this;
    }

    Buffer &Buffer::operator=(Buffer &&other) noexcept {
        if (this != &other) {
            free(m_Data);

            m_Data = other.m_Data;
            m_Size = other.m_Size;
            other.m_Data = nullptr;
            other.m_Size = 0;
        }

        return *this;
    }

    void Buffer::SetData(const void *data, const size_t length) const {
        if (length > m_Size || m_Data == nullptr || data == nullptr) {
            return;
        }

        memcpy(m_Data, data, length);
    }

    void *Buffer::Get() const { return m_Data; }

    size_t Buffer::GetSize() const { return m_Size; }

    void *Buffer::At(const size_t offset) const {
        if (offset >= m_Size) {
            return nullptr;
        }

        return static_cast<char *>(m_Data) + offset;
    }
} // namespace Pixf::Core::Memory
