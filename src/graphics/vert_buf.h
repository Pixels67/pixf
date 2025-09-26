#pragma once

#include <vector>

namespace Engine::Graphics {
    class VertBuf {
    public:
        VertBuf() = default;

        VertBuf(const std::vector<float> &vertices, const std::vector<unsigned int> &indices, unsigned int usage);


        VertBuf(const VertBuf &other) = delete;

        VertBuf &operator=(const VertBuf &other) = delete;

        VertBuf(VertBuf &&other) noexcept ;

        VertBuf &operator=(VertBuf &&other) noexcept ;


        ~VertBuf();


        void Bind() const;

        static void Unbind();


        [[nodiscard]] unsigned int GetIndexCount() const;

    private:
        unsigned int m_Vbo = 0;
        unsigned int m_Ebo = 0;

        unsigned int m_IndexCount = 0;
    };
} // namespace Engine::Graphics
