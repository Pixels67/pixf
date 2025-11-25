#include "Application/Application.hpp"
#include "Dylib.hpp"

using namespace Pixf::Runtime;
using namespace Pixf::Core::Application;

int main(const int argc, char *argv[]) {
    std::set_terminate(Pixf::Core::Error::HandleTerminate);

    if (argc != 2) {
        return 1;
    }

    const char *dylibPath = argv[1];

    Dylib dylib = Dylib::Load(dylibPath);

    Application *app = dylib.GetFunction<Application *(*) ()>("CreateApplication")();

    app->Run();

    dylib.GetFunction<void (*)(Application *)>("DestroyApplication")(app);

    return 0;
}
