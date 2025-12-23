#include "GuiRenderArchive.hpp"

namespace Pixf::Core::Serial {
    bool GuiRenderArchive::IsOutput() { return true; }
    bool GuiRenderArchive::IsInput() { return false; }

    void GuiRenderArchive::Recurse(const std::string &key, const std::function<void(GuiRenderArchive &)> &func) {
        GuiRenderArchive inner;
        func(inner);
    }
}