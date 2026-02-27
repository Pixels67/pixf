#ifndef FLK_BUFFER_HPP
#define FLK_BUFFER_HPP

#include "Common.hpp"

namespace Flock::Memory {
    class FLK_API Buffer {
    public:
        Buffer() = default;

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
        void ReadTo(void *buffer) const;
        void ReadTo(void *buffer, size_t length) const;
        void ReadTo(void *buffer, size_t offset, size_t length) const;

        [[nodiscard]] void *Get() const;
        [[nodiscard]] void *Get(size_t offset) const;

        [[nodiscard]] size_t GetSize() const;
        [[nodiscard]] void *At(size_t offset) const;

        void Clear();

    private:
        void *m_Data = nullptr;
        size_t m_Size = 0;
    };
}

#endif //FLK_BUFFER_HPP