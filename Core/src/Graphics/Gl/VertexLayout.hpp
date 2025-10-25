#ifndef VERTEXLAYOUT_HPP
#define VERTEXLAYOUT_HPP

#include <vector>

#include "../../Common.hpp"
#include "Gl.hpp"

namespace Pixf::Core::Graphics::Gl {
    struct VertexLayoutElement {
        unsigned int type;
        unsigned int count;
        unsigned int offset;
        bool normalized;

        size_t GetSize() const;
    };

    class VertexLayout {
    public:
        template<typename T>
        void Push(unsigned int count) {
            ASSERT(false, "Not implemented");
        }

        template<>
        void Push<float>(const unsigned int count) {
            m_Elements.push_back({.type = GL_FLOAT, .count = count, .offset = m_Stride, .normalized = false});
            m_Stride += count * sizeof(float);
        }

        template<>
        void Push<int>(const unsigned int count) {
            m_Elements.push_back({.type = GL_INT, .count = count, .offset = m_Stride, .normalized = false});
            m_Stride += count * sizeof(int);
        }

        template<>
        void Push<unsigned int>(const unsigned int count) {
            m_Elements.push_back({.type = GL_UNSIGNED_INT, .count = count, .offset = m_Stride, .normalized = false});
            m_Stride += count * sizeof(unsigned int);
        }

        template<>
        void Push<char>(const unsigned int count) {
            m_Elements.push_back({.type = GL_BYTE, .count = count, .offset = m_Stride, .normalized = true});
            m_Stride += count * sizeof(char);
        }

        unsigned int GetStride() const;
        std::vector<VertexLayoutElement> GetElements() const;

        void Bind() const;
        void Unbind() const;

    private:
        std::vector<VertexLayoutElement> m_Elements;
        unsigned int m_Stride = 0;
    };
} // namespace Pixf::Core::Graphics::Gl

#endif // VERTEXLAYOUT_HPP
