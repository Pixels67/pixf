<h3 align="center">
  C++ game framework / engine.
</h3>

---

A C++ game framework/engine using OpenGL 3.3, currently only supports desktop.
You can either build the *Core* library and use it like a library, or build it with the *Editor* and *Runtime* and use it like an engine.

> [!IMPORTANT]  
> This is just a hobby project and most of it is not finished yet.  
> Expect a lot of features missing.

## Features
- [x] ECS
- [x] PBR
- [ ] IBL
- [x] Keyboard & Mouse Input
- [x] Audio
- [ ] Serialization
- [ ] Immediate mode GUI
- [ ] Retained mode GUI
- [ ] Editor 

## Building from Source

Make sure you have a C++ compiler and CMake installed and run the following commands:

```sh
git clone https://github.com/Pixels67/pixf.git
cd pixf
git checkout refactor

cmake -S . -B build/
cmake --build build/ --config Release # Or Debug
```

> [!NOTE]  
> On Linux you need to install the dependencies for [GLFW](https://www.glfw.org/docs/latest/compile.html).
>
> > To compile for both Wayland and X11, you need to have the X11, Wayland and xkbcommon development packages
> > installed.  
> > On Debian and derivatives, you will need the `libwayland-dev` and `libxkbcommon-dev` packages to compile for Wayland
> > and the `xorg-dev` meta-package to compile for X11.
> > ```sh
> > sudo apt-get install libwayland-dev libxkbcommon-dev xorg-dev
> > ```

## Gallery