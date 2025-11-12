#include "Application.hpp"
#include "Debug/Logger.hpp"

#include "Dylib.hpp"

using namespace Pixf::Runtime;

int main(const int argc, char *argv[]) {
    if (argc != 2) {
        PIXF_LOG_FATAL("Usage: ", argv[0], " <path_to_dll>");
        return 1;
    }

    const char *dylibPath = argv[1];
    PIXF_LOG_INFO("Loading Dylib: ", dylibPath, DYLIB_EXT);

    Dylib dylib = Dylib::Load(dylibPath);

    Pixf::Core::Application *app = dylib.GetFunction<Pixf::Core::Application *(*) ()>("CreateApplication")();

    app->Run();

    dylib.GetFunction<void (*)(Pixf::Core::Application *)>("DestroyApplication")(app);

    return 0;
}
