#ifndef FLK_EVENTREGISTRY_HPP
#define FLK_EVENTREGISTRY_HPP

#include <functional>
#include <string>
#include <unordered_map>

#include "Common.hpp"

namespace Flock::Event {
    using Callback = std::function<void()>;

    class FLK_API EventRegistry {
        std::unordered_map<std::string, Callback> m_Callbacks;

    public:
        void Add(const std::string &key, const Callback &callback);
        bool Remove(const std::string &key);
        bool Invoke(const std::string &key) const;

        void Clear();
    };
}

#endif //FLK_EVENTREGISTRY_HPP
