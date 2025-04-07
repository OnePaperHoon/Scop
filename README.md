# 🌀 SCOP - Simple OpenGL 3D Viewer

A minimal OpenGL-based 3D object viewer written in C++

---

## Demo
![Scop Demo](/Demo/Scop.gif)

## 📦 About

SCOP is a small 3D rendering application that loads `.obj` files, renders them using OpenGL, and allows camera movement, object manipulation, and toggling between texture and color modes.

This project is entirely built **without external math or model libraries** (no GLM, no Assimp), and **only uses OpenGL and basic windowing**.

---

## ✅ Features

### ✔️ Required

- 🧱 Loads and renders `.obj` 3D models
- 🎨 Displays different colors per face
- 🔄 Object auto-rotates around its center
- 🕹️ Move object freely along X, Y, Z axes
- ⌨️ Keyboard controls for translation
- 🎭 Toggle between solid color and texture
  - Smooth transition between modes
- 🔲 Perspective projection enabled
- 📂 Custom-made `.obj` parser
- 🧮 Self-implemented matrix math (no glm)
- 📜 Shader loader written from scratch

---

### 🌟 Bonus (Optional Features)

- ✔️ Gracefully handles malformed `.obj` files (e.g. open teapot mesh)
- ✔️ Texture mapping with correct UVs
- ✔️ Face normal smoothing
- ✔️ Display mode toggle (wireframe, normal debug)
- ✔️ Mouse camera control (arcball rotation)

---

## 🖥️ Controls

| Key         | Action                          |
|-------------|---------------------------------|
| `WASD`      | Move object (X/Y axis)          |
| `Q / E`     | Move object (Z axis)            |
| `R`         | Reset object position           |
| `T`         | Toggle between texture / color  |
| `1`         | Change View Option (Wire / Fill)|
| `ESC`       | Quit the application            |

---

## 🛠️ Build
You need first GLFW Install

```bash
make
./scop teapot.obj
or
./scop
```
___


# 📁 Project Structure
```
├── Makefile
├── README.md
├── library
│   └── stb_image.h
├── resources <- add Your obj file
│   ├── 42.mtl
│   ├── 42.obj
│   ├── sample.png
│   ├── teapot.obj
│   ├── teapot2.mtl
│   └── teapot2.obj
└── srcs
    ├── InputHandler.cpp
    ├── InputHandler.hpp
    ├── Material.cpp
    ├── Material.hpp
    ├── MathUtils.hpp
    ├── Model.cpp
    ├── Model.hpp
    ├── Shader.cpp
    ├── Shader.hpp
    ├── UVRotation.hpp
    ├── main.cpp
    └── shaders
        ├── shader_frag.glsl
        └── shader_vert.glsl
```
