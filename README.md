# 3D Lantern

## Description

3D Lantern is a 3D modeling and rendering application built with OpenGL, SDL2, and ImGui. It provides an interactive environment for creating and visualizing 3D scenes, alongside a custom CPU-side ray tracing engine for high-quality offline image rendering.

The project also includes a custom scene description language, allowing users to define and render complex scenes programmatically via `.lnt` files.

<p align="center">
  <img src="https://i.imgur.com/p7g4IKE.png" width="1270" height=720/>
</p>

<p align="center">
  <img src="https://imgur.com/GsnHXYD.png" width="1270" height=720/>
</p>

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
│   ├── lantern.cpp        # interpreter program entry point
│   ├── lantern3D.cpp      # Main program entry point
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


# How To Use
Within `3d_lantern/build/bin/` you will find 2 executable programs: `lantern`, which is the interpreter, and `lantern3D`, which is the main program with a 3D modeling tool. 


# lantern and Scene Description Language Reference

This document describes the scene file format (.lnt) used to define rendering settings, cameras, textures, materials, objects, and lights.

---

## Overview

A scene file is composed of **blocks**. Each block begins with a keyword (e.g. `setting`, `camera`, `object`) followed by `{` and `}`. Inside the block, properties are declared as key–value pairs, one per line.

```
block_type {
    property_name  value(s)
}
```

String values (e.g. file paths) must be wrapped in double quotes. All other values (numbers, booleans, identifiers) are unquoted.

---

### `setting`

Defines global render parameters. There should be exactly one `setting` block per scene file.

| Property       | Type    | Description                                      |
|----------------|---------|--------------------------------------------------|
| `width`        | integer | Output image width in pixels                     |
| `height`       | integer | Output image height in pixels                    |
| `samples`      | integer | Number of ray samples per pixel (higher = less noise) |
| `depth`        | integer | Maximum ray bounce depth                         |
| `aspect_ratio` | float   | Width-to-height ratio (width / height)           |
| `background`   | RGB     | Background color as three floats (R G B, 0.0–1.0) |

**Example:**
```
setting {
    width        1280
    height       720
    samples      500
    depth        30
    aspect_ratio 1.7778
    background   0.0 0.0 0.0
}
```

---

### `camera`

Defines the camera used for the render. There should be exactly one `camera` block.

| Property         | Type  | Description                                                   |
|------------------|-------|---------------------------------------------------------------|
| `position`       | XYZ   | Camera position in world space                                |
| `lookat`         | XYZ   | Point the camera is aimed at                                  |
| `up_view`        | XYZ   | The "up" direction vector for camera orientation              |
| `vfov`           | float | Vertical field of view in degrees                             |
| `defocus_angle`  | float | Depth-of-field blur angle; `0.0` disables defocus            |
| `focus_distance` | float | Distance from the camera to the plane of perfect focus        |

**Example:**
```
camera {
    position       0.0 0.0  1.0
    lookat         0.0 0.0 -1.0
    up_view        0.0 3.0  0.0
    vfov           60.0
    defocus_angle  0.0
    focus_distance 10.0
}
```

---

### `texture`

Defines a named texture that can be referenced by materials. Multiple `texture` blocks are allowed.

| Property      | Type   | Required by         | Description                                    |
|---------------|--------|---------------------|------------------------------------------------|
| `name`        | string | all                 | Unique identifier used in `texture_ref`        |
| `type`        | enum   | all                 | Texture type (see below)                       |
| `scale`       | float  | procedural types    | Controls the frequency/size of the pattern     |
| `even`        | RGB    | `checker` only      | Color for even tiles                           |
| `odd`         | RGB    | `checker` only      | Color for odd tiles                            |
| `file`        | string | `image` only        | Path to the image file (quoted)                |

### Texture Types

| Type          | Description                                      |
|---------------|--------------------------------------------------|
| `marble`      | Procedural marble pattern using Perlin noise     |
| `wood`        | Procedural wood-grain pattern using Perlin noise |
| `turbulence`  | Raw Perlin turbulence noise                      |
| `checker`     | Alternating two-color checkerboard               |
| `image`       | Loads an image file as a texture                 |

**Examples:**
```
texture {
    name perlin_marble
    type marble
    scale 10.0
}

texture {
    name checker_tx
    type checker
    even  1.0 1.0 1.0
    odd   0.0 0.0 0.0
    scale 4.0
}

texture {
    name bg_image
    type image
    file "texture-background.jpg"
}
```

---

### `material`

Defines a named material that can be referenced by objects. Multiple `material` blocks are allowed.

| Property      | Type   | Required by          | Description                                                      |
|---------------|--------|----------------------|------------------------------------------------------------------|
| `name`        | string | all                  | Unique identifier used in `material_ref`                         |
| `type`        | enum   | all                  | Material type (see below)                                        |
| `color`       | RGB    | `metallic`           | Base color as R G B floats (0.0–1.0)                             |
| `fuzz`        | float  | `metallic`           | Reflection blur amount (0.0 = mirror, 1.0 = fully diffuse)       |
| `texture_ref` | string | `lambertian` (opt.)  | Name of a texture to use instead of a solid color                |
| `ior`         | float  | `dielectric`         | Index of refraction (e.g. glass ≈ 1.5)                          |

### Material Types

| Type          | Description                                                          |
|---------------|----------------------------------------------------------------------|
| `lambertian`  | Diffuse (matte) surface; accepts a `texture_ref` or default color    |
| `metallic`    | Reflective metal surface; requires `color` and `fuzz`                |
| `dielectric`  | Transparent/refractive surface (glass, water); requires `ior`        |

**Examples:**
```
material {
    name center
    type metallic
    color 0.4 0.4 0.8
    fuzz  0.0
}

material {
    name left
    type lambertian
    texture_ref perlin_marble
}

material {
    name right
    type dielectric
    ior 1.490
}
```

---

### `object`

Places a geometric primitive into the scene. Multiple `object` blocks are allowed.

| Property       | Type    | Required by      | Description                                               |
|----------------|---------|------------------|-----------------------------------------------------------|
| `name`         | string  | all              | Unique identifier for the object                          |
| `type`         | enum    | all              | Geometry type (see below)                                 |
| `material_ref` | string  | all              | Name of the material to apply                             |
| `translate`    | XYZ     | all              | Position offset in world space                            |
| `scale`        | XYZ     | all              | Scale along each axis; use uniform values for spheres     |
| `capped`       | boolean | `cylinder`/`cone`| Whether to close the open ends with flat caps             |

### Object Types

| Type       | Description                                                                  |
|------------|------------------------------------------------------------------------------|
| `sphere`   | A sphere. Scale uniformly (X = Y = Z) to control radius                     |
| `cylinder` | A cylinder aligned along the Y axis. Use `capped` to close the ends         |
| `cone`     | A cone with its apex at the top, aligned along the Y axis. Use `capped`     |

> **Tip:** For cylinders and cones, the X and Z scale values control the radius and the Y scale controls the height.

**Examples:**
```
object {
    name sphere_center
    type sphere
    material_ref center
    translate  0.0  -0.2  -1.0
    scale      0.3   0.3   0.3
}

object {
    name cyl_left
    type cylinder
    material_ref cyl_left
    capped true
    translate -1.5  -0.05  -1.5
    scale      0.18   0.95   0.18
}

object {
    name cone_right
    type cone
    material_ref cone_right
    capped true
    translate  1.5  0.5  -1.0
    scale      0.364 1.0  0.364
}
```

---

### `light`

Defines a light source in the scene. Multiple `light` blocks are allowed.

| Property    | Type  | Description                                         |
|-------------|-------|-----------------------------------------------------|
| `type`      | enum  | Light type (see below)                              |
| `position`  | XYZ   | World-space position of the light                   |
| `color`     | RGB   | Light color as R G B floats (0.0–1.0)               |
| `intensity` | float | Brightness multiplier                               |

### Light Types

| Type    | Description                               |
|---------|-------------------------------------------|
| `point` | Omnidirectional light from a single point |

**Example:**
```
light {
    type      point
    position -5.0 5.0 -1.0
    color      1.0 1.0  1.0
    intensity  2.5
}
```

---

### Value Types

| Type    | Format              | Example              |
|---------|---------------------|----------------------|
| integer | Whole number        | `1280`               |
| float   | Decimal number      | `0.5`, `60.0`        |
| RGB     | Three floats        | `0.4 0.4 0.8`        |
| XYZ     | Three floats        | `-1.5 0.05 -1.5`     |
| boolean | `true` or `false`   | `true`               |
| string  | Quoted text         | `"texture.jpg"`      |
| enum    | Unquoted identifier | `sphere`, `metallic` |

---

### Block Order & Rules

- Block order in the file does not matter for most parsers, but the conventional order is: `setting` → `camera` → `texture` → `material` → `object` → `light`.
- `name` values must be **unique within their block type** (e.g. two textures cannot share a name, but a texture and a material can).
- `texture_ref` and `material_ref` must refer to a name that is defined elsewhere in the file.
- There must be exactly **one `setting` block** and **one `camera` block**.


## lantern3D — 3D Modeling Tool

### Setup

Before launching `lantern3D`, create the following directories in the same location as the executable:
3d_lantern/build/bin/
├── lantern3D
├── img/          ← rendered images are saved here
└── ressources/   ← place your .obj mesh files here

> The program will not run correctly if these directories are missing.

### Controls

#### Selection

| Input | Action |
|---|---|
| `Tab` | Cycle to the next object in the scene |
| `Right Click` | Pick an object or light under the cursor |
| Right click on empty space | Deselect current selection |

#### Modes

Switch the active editing mode with these keys:

| Key | Mode | Description |
|---|---|---|
| `P` | Camera | Move and look around the scene |
| `T` | Translate | Move the selected object or light |
| `R` | Rotate | Rotate the selected object |
| `S` | Scale | Scale the selected object |

> Rotate and Scale are not available for lights — only Translate applies.

#### Axis Lock

Constrain transformations to a single axis. Press again or press `N` to go back to free movement:

| Key | Axis |
|---|---|
| `X` | Lock to X axis |
| `Y` | Lock to Y axis |
| `Z` | Lock to Z axis |
| `N` | Free (no axis lock) |

#### Camera (Camera mode)

| Input | Action |
|---|---|
| `↑ ↓ ← →` | Move forward / backward / left / right |
| `Space` | Move up |
| `Left Alt` | Move down |
| `Left Shift` + movement | Sprint (3× speed) |
| `Left Mouse` + drag | Look around |

#### Transform (Translate / Rotate / Scale modes)

| Input | Action |
|---|---|
| `↑` | Move / rotate / grow in the positive direction |
| `↓` | Move / rotate / shrink in the negative direction |
| `← →` | Move on X axis (Translate, free mode only) |
| `Left Ctrl` + any | Precision mode (5× slower) |
