#include <SDL3/SDL.h>
#include <bgfx/bgfx.h>

#include "common.hpp"

using namespace flock;

i32 main() {
    SDL_InitSubSystem(SDL_INIT_VIDEO);
    SDL_Window *window = SDL_CreateWindow("Flock", 1080, 720, 0);

    const SDL_PropertiesID properties = SDL_GetWindowProperties(window);

    bgfx::Init init;

#if SDL_PLATFORM_WIN32
    init.platformData.nwh = SDL_GetPointerProperty(properties, SDL_PROP_WINDOW_WIN32_HWND_POINTER, nullptr);
#elif SDL_PLATFORM_MACOS
    init.platformData.nwh = SDL_GetPointerProperty(properties, SDL_PROP_WINDOW_COCOA_WINDOW_POINTER, nullptr);
#elif SDL_PLATFORM_LINUX
    const char *driver = SDL_GetCurrentVideoDriver();
    if (SDL_strcmp(driver, "wayland") == 0) {
        init.platformData.nwh = SDL_GetPointerProperty(properties, SDL_PROP_WINDOW_WAYLAND_SURFACE_POINTER, nullptr);
        init.platformData.ndt = SDL_GetPointerProperty(properties, SDL_PROP_WINDOW_WAYLAND_DISPLAY_POINTER, nullptr);
        init.platformData.type = bgfx::NativeWindowHandleType::Wayland;
    } else if (SDL_strcmp(driver, "x11")) {
        init.platformData.nwh = (void *) SDL_GetNumberProperty(properties, SDL_PROP_WINDOW_X11_WINDOW_NUMBER, 0);
        init.platformData.ndt = SDL_GetPointerProperty(properties, SDL_PROP_WINDOW_X11_DISPLAY_POINTER, nullptr);
    } else {
        FLK_EXPECT(false, "Unknown video driver");
    }
#endif

    init.resolution.width = 1080;
    init.resolution.height = 720;
    init.resolution.reset = BGFX_RESET_VSYNC;
    init.debug = BGFX_DEBUG_NONE;

    bgfx::init(init);

    bgfx::setViewRect(0, 0, 0, 1080, 720);
    bgfx::setViewClear(0, BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH, 0x11222FFF);

    bool quit = false;
    while (!quit) {
        SDL_Event e;
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_EVENT_QUIT) {
                quit = true;
            }
        }

        bgfx::touch(0);
        bgfx::frame();
    }

    bgfx::shutdown();
    SDL_DestroyWindow(window);
    SDL_Quit();
}
