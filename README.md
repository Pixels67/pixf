# PiXF

![PiXF](PiXF.gif)  
C++ game framework using OpenGL 3.3.

## Building from Source

Make sure you have a C++ compiler (Clang preferred) and CMake installed and run the following commands:

```sh
git clone --recursive https://github.com/Pixels67/pixf.git
cd pixf;

cmake -S . -B build/
cmake --build build/ --config Release
```

For a Debug build run the following commands:

```sh
git clone --recursive https://github.com/Pixels67/pixf.git
cd pixf;

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
