#include "Dylib.hpp"

using namespace Pixf::Runtime;

int main(const int argc, char *argv[]) {
    if (argc != 2) {
        return 1;
    }

    const char *dylibPath = argv[1];

    Dylib dylib = Dylib::Load(dylibPath);

    return 0;
}
