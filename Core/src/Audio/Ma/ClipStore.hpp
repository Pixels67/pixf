#ifndef PIXF_CLIPSTORE_HPP
#define PIXF_CLIPSTORE_HPP

#include "Clip.hpp"
#include "Handle.hpp"
#include "Common.hpp"
#include "Error/Error.hpp"

namespace Pixf::Core::Audio::Ma {
    class PIXF_API ClipStoreError final : public Error::Error {
    public:
        using Error::Error;
    };

    class PIXF_API ClipStore {
    public:
        struct PIXF_API Slot {
            Clip clip;
            uint8_t version;
            bool active;
        };

        ClipHandle Create(Clip &&clip);
        void Destroy(ClipHandle handle);

        Clip &Get(ClipHandle handle);

    private:
        std::vector<Slot> m_Clips;

        std::pair<uint32_t, Slot &> GetSlot();
    };
} // namespace Pixf::Core::Audio::Ma

#endif // PIXF_CLIPSTORE_HPP
