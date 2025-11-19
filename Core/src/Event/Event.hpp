#ifndef PIXF_EVENT_HPP
#define PIXF_EVENT_HPP

#include "Common.hpp"

namespace Pixf::Core::Event {
    class PIXF_API Event {
    public:
        virtual ~Event() = default;
    };
} // namespace Pixf::Core::Event

#endif // PIXF_EVENT_HPP
