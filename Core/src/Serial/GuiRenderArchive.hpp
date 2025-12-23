#ifndef PIXF_GUIRENDERARCHIVE_HPP
#define PIXF_GUIRENDERARCHIVE_HPP

#include "Gui/Gui.hpp"
#include "Serial.hpp"

namespace Pixf::Core::Serial {
    class PIXF_API GuiRenderArchive {
    public:
        static bool IsOutput();
        static bool IsInput();

        void Recurse(const std::string &key, const std::function<void(GuiRenderArchive &)> &func);

        template<typename T>
        bool operator()(const std::string &key, T &value) {
            if constexpr (Serializable<GuiRenderArchive, T>) {
                Recurse(key, [&](GuiRenderArchive &ar) { Serialize(ar, value); });
            } else {
                Gui::Text("{}: {}", key, value);
            }

            return true;
        }

        template<typename T>
        bool operator()(const std::string &key, const std::vector<T> &array) {
            if constexpr (Serializable<GuiRenderArchive, T>) {
                for (size_t i = 0; i < array.size(); i++) {
                    Gui::Text("- {}", array[i]);
                }
            } else {
                for (size_t i = 0; i < array.size(); i++) {
                    Gui::Text("- {}", array[i]);
                }
            }

            return true;
        }

        template<typename T>
        bool AddArray(const std::string &key,
                      const std::vector<T> &array,
                      std::function<void(GuiRenderArchive &, T &)> func) {
            Gui::Text("-- {} --", key);
            for (size_t i = 0; i < array.size(); i++) {
                func(*this, const_cast<T &>(array[i]));
                Gui::Text("{}: {}", array[i]);
            }

            return true;
        }
    };
} // namespace Pixf::Core::Serial

#endif // PIXF_GUIRENDERARCHIVE_HPP
