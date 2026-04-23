# 3D Lantern

## Description

3D Lantern is a 3D modeling and rendering application built with OpenGL, SDL2, and ImGui. It provides an interactive environment for creating and visualizing 3D scenes, alongside a custom CPU-side ray tracing engine for high-quality offline image rendering.

The project also includes a custom scene description language, allowing users to define and render complex scenes programmatically via `.lnt` files.

## Features

- Real-time 3D rendering using OpenGL
- Interactive graphical interface powered by ImGui
- Custom CPU-side ray tracer for offline rendering
- Scene description language (`.lnt`) for procedural scene creation
- Support for geometric primitives (spheres, planes, cylinders, cones, triangles, meshes)
- BVH acceleration structure for efficient ray-mesh intersection
- Multithreaded rendering with progress reporting
- Configurable camera and lighting system
- OBJ mesh loading via tinyobjloader
- Texture support via stb_image

## Requirements

- C++20 or later
- CMake 3.23 or later
- SDL2
- OpenGL

> All other dependencies (ImGui, GLAD, stb, tinyobjloader) are bundled under `external/`.

## Building

From the root of the project, configure and build using CMake:

```bash
cmake -B build
cmake --build build
```

The main executable will be output to:

```
build/bin/lantern
```

### Running Tests

```bash
cd build && ctest
```

Or run individual test binaries from `build/test/`.

### Running Examples

Example binaries are output to `build/example/`. Each example renders a scene and writes a `.ppm` image to the `example/` directory.

```bash
./build/example/boxes
```

## Project Structure

```
3d-lantern/
├── src/
│   ├── lantern.cpp        # Application entry point
│   ├── application/       # Window, event loop, UI
│   ├── modeling/          # OpenGL scene, renderer, mesh
│   ├── ray_tracer/        # Ray tracing engine, BVH, materials
│   ├── interpreter/       # .lnt scene description language
│   ├── assets/            # Texture and resource loading
│   └── math/              # Custom math library (vectors, matrices)
├── example/               # Example scenes and output images
├── test/                  # Unit tests
├── shaders/               # GLSL shaders
├── resources/             # OBJ/MTL assets
├── img/                   # Texture images
└── external/              # Bundled dependencies
```