#ifndef PIXF_MATERIALSTORE_HPP
#define PIXF_MATERIALSTORE_HPP

#include "Common.hpp"
#include "Material.hpp"
#include "Error/Error.hpp"

namespace Pixf::Core::Graphics {
    class MaterialStoreError final : public Error::Error {
    public:
        using Error::Error;
    };

    class PIXF_API MaterialStore {
    public:
        struct PIXF_API Slot {
            Material material;
            uint8_t version;
            bool active;
        };

        MaterialHandle Create(Material &&material);
        void Destroy(MaterialHandle handle);

        Material &Get(MaterialHandle handle);

    private:
        std::vector<Slot> m_Materials;

        std::pair<uint32_t, Slot &> GetSlot();
    };
} // namespace Pixf::Core::Graphics

#endif // PIXF_MATERIALSTORE_HPP
