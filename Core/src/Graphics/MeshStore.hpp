#ifndef PIXF_MESHMANAGER_HPP
#define PIXF_MESHMANAGER_HPP

#include "Common.hpp"
#include "Error/Error.hpp"
#include "Handle.hpp"
#include "Mesh.hpp"

namespace Pixf::Core::Graphics {
    class PIXF_API MeshStoreError final : public Error::Error {
    public:
        using Error::Error;
    };

    class PIXF_API MeshStore {
    public:
        struct Slot {
            Mesh mesh;
            uint8_t version;
            bool active;
        };

        MeshHandle Load(const MeshData &meshData);
        void Unload(MeshHandle handle);

        Mesh &Get(MeshHandle handle);

    private:
        std::vector<Slot> m_Meshes;

        std::pair<uint32_t, Slot &> GetSlot();
    };
} // namespace Pixf::Core::Graphics

#endif // PIXF_MESHMANAGER_HPP
