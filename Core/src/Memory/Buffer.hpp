#ifndef PIXF_MEMORY_BUFFER_HPP
#define PIXF_MEMORY_BUFFER_HPP

#include "Common.hpp"

namespace Pixf::Core::Memory {
    class PIXF_API Buffer {
    public:
        explicit Buffer(size_t size);
        Buffer(const void *src, size_t size);

        template<typename T>
        Buffer(const std::vector<T> &vector) {
            if (vector.empty()) {
                return;
            }

            *this = Buffer(vector.data(), vector.size() * sizeof(vector[0]));
        }

        Buffer(const Buffer &other);
        Buffer(Buffer &&other) noexcept;

        Buffer &operator=(const Buffer &other);
        Buffer &operator=(Buffer &&other) noexcept;

        ~Buffer();

        void SetData(const void *data, size_t length) const;
        void *Get() const;

        size_t GetSize() const;
        void *At(size_t offset) const;

    private:
        void *m_Data = nullptr;
        size_t m_Size = 0;
    };
} // namespace Pixf::Core::Memory

#endif // PIXF_MEMORY_BUFFER_HPP
