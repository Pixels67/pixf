<h3 align="center">
  <img width="896" height="128" alt="flock_banner" src="https://github.com/user-attachments/assets/0c0ba71e-5e0f-4163-9f95-3f76514b31c1" />
  <br/>
  <br/>
  Flock
</h3>

---

A C++ game engine using OpenGL 3.3 inspired by Bevy (hence the name).
You can either build the *Core* library and use it like a library,
or build it with the *Editor* and *Runtime* and use it like an engine.

> [!IMPORTANT]  
> This is just a hobby project and most of it is not finished yet.  
> Expect a lot of features missing.

## Features
- [x] ECS
- [x] PBR
- [ ] IBL
- [x] Keyboard & Mouse Input
- [x] Audio
- [x] Serialization
- [ ] Immediate mode GUI
- [ ] Retained mode GUI
- [ ] Editor 

## Building from Source

Make sure you have a C++ compiler and CMake installed and run the following commands:

```sh
git clone https://github.com/Pixels67/Flock.git
cd Flock

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
<img width="912" height="940" alt="rend2" src="https://github.com/user-attachments/assets/dd9a0383-4a8b-4e07-8be4-4b73c946f972" />
<img width="912" height="940" alt="rend1" src="https://github.com/user-attachments/assets/a9bd5beb-4f7e-4a2d-a5d5-0a149a6538ab" />
