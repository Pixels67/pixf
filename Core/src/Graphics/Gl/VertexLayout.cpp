#include "VertexLayout.hpp"

#include <vector>

#include "Gl.hpp"

namespace Pixf::Core::Graphics::Gl {
    size_t VertexLayoutElement::GetSize() const {
        switch (type) {
            case GL_FLOAT:
                return sizeof(float);
            case GL_UNSIGNED_INT:
                return sizeof(unsigned int);
            case GL_INT:
                return sizeof(int);
            case GL_BYTE:
                return sizeof(char);
            default:
                return 0;
        }
    }

    unsigned int VertexLayout::GetStride() const { return m_Stride; }
    std::vector<VertexLayoutElement> VertexLayout::GetElements() const { return m_Elements; }

    void VertexLayout::Bind() const {
        unsigned int offset = 0;
        for (size_t i = 0; i < m_Elements.size(); i++) {
            GL_CALL(glVertexAttribPointer(i, m_Elements.at(i).count, m_Elements.at(i).type, m_Elements.at(i).normalized,
                                          m_Stride, reinterpret_cast<void *>(static_cast<size_t>(offset))));
            GL_CALL(glEnableVertexAttribArray(i));
            offset += m_Elements.at(i).count * m_Elements.at(i).GetSize();
        }
    }

    void VertexLayout::Unbind() const {
        for (size_t i = 0; i < m_Elements.size(); i++) {
            std::cout << "unbind\n";
            GL_CALL(glDisableVertexAttribArray(i));
        }
    }
} // namespace Pixf::Core::Graphics::Gl
