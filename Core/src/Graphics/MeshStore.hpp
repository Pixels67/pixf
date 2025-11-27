#ifndef PIXF_MESHMANAGER_HPP
#define PIXF_MESHMANAGER_HPP

#include "Handle.hpp"
#include "Common.hpp"
#include "Error/Error.hpp"
#include "Mesh.hpp"

namespace Pixf::Core::Graphics {
    class PIXF_API MeshStoreError final : public Error::Error {
    public:
        using Error::Error;
    };

    class PIXF_API MeshStore {
    public:
        struct PIXF_API Slot {
            Mesh mesh;
            uint8_t version;
            bool active;
        };

        MeshStore() = default;

        MeshStore(const MeshStore &) = delete;
        MeshStore(MeshStore &&) noexcept = default;
        MeshStore &operator=(const MeshStore &) = delete;
        MeshStore &operator=(MeshStore &&) noexcept = default;

        ~MeshStore() = default;

        MeshHandle Create(Mesh &&mesh);
        void Destroy(MeshHandle handle);

        Mesh &Get(MeshHandle handle);

    private:
        std::vector<Slot> m_Meshes;

        std::pair<uint32_t, Slot &> GetSlot();
    };
} // namespace Pixf::Core::Graphics

#endif // PIXF_MESHMANAGER_HPP
