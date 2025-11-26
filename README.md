<h3 align="center">
  <img align="center" src="PiXF.gif"><br><br>
  C++ game framework / engine.
</h3>

---

A C++ game framework/engine using OpenGL 3.3, currently only supports desktop.
You can either build the core library and use it like a library, or build it with the Editor and Runtime (Note: both are WIP) and use it like an engine.

> [!WARNING]  
> This is just a hobby project and most of it is not finished yet.  
> Expect a lot of features missing.

## Building from Source

Make sure you have C++ compiler and CMake installed and run the following commands:

```sh
git clone https://github.com/Pixels67/pixf.git
cd pixf

cmake -S . -B build/
cmake --build build/ --config Release
```

For a Debug build run the following commands:

```sh
git clone https://github.com/Pixels67/pixf.git
cd pixf

cmake -S . -B build/
cmake --build build/ --config Debug
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
