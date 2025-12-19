<h3 align="center">
  <img align="center" src="PiXF.gif"><br><br>
  C++ game framework / engine.
</h3>

---

A C++ game framework/engine using OpenGL 3.3, currently only supports desktop.
You can either build the *Core* library and use it like a library, or build it with the *Editor* and *Runtime* and use it like an engine.

> [!IMPORTANT]  
> This is just a hobby project and most of it is not finished yet.  
> Expect a lot of features missing.

## Features
- [x] Phong Rendering
- [ ] PBR
- [x] ECS (using EnTT)
- [ ] Audio
- [x] Serialization
- [x] Immediate mode GUI
- [ ] Retained mode GUI
- [x] Keyboard & Mouse Input
- [ ] Editor 

## Building from Source

Make sure you have a C++ compiler and CMake installed and run the following commands:

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

## Gallery  

<img width="912" height="740" alt="Dragon" src="https://github.com/user-attachments/assets/eba021cb-140f-4fc7-bfe0-c34d9b9579d5" />
<img width="912" height="740" alt="Dragons" src="https://github.com/user-attachments/assets/32d914e1-8d03-4e70-8554-4b87b32ec2e7" />
<img width="912" height="740" alt="Duck" src="https://github.com/user-attachments/assets/2c1495f9-2fe3-4db5-a07b-0b422982e3b7" />
<img width="912" height="740" alt="Backpack" src="https://github.com/user-attachments/assets/6646af09-9db3-4616-9303-0feae00a8ed7" />
