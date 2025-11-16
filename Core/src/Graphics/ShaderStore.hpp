#ifndef PIXF_SHADERMANAGER_HPP
#define PIXF_SHADERMANAGER_HPP

#include <string>

#include "Gl/Shader.hpp"
#include "Handle.hpp"

namespace Pixf::Core::Graphics {
    class PIXF_API ShaderStoreError final : public Error::Error {
        using Error::Error;
    };

    class PIXF_API ShaderStore {
    public:
        struct Slot {
            Gl::Shader shader;
            uint8_t version;
            bool active;
        };

        ShaderHandle Load(const std::string &vertexSource, const std::string &fragmentSource);
        void Unload(ShaderHandle handle);

        Gl::Shader &Get(ShaderHandle handle);

    private:
        std::vector<Slot> m_Shaders;

        std::pair<uint32_t, Slot &> GetSlot();
    };
} // namespace Pixf::Core::Graphics

#endif // PIXF_SHADERMANAGER_HPP
