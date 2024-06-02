# Yet Another Game Engine
A C++ game engine / Work-in-progress

## Overview
The goal of this project is to implement a 3D game engine from scratch (within reason, I'm happy to use libraries for stuff like window creation or reading PNG files). I don't expect to use this for developing any actual games but rather treat it as a learning experience for graphics programming and C++ in general.

## Subsystems
The engine is built with a focus on modularity and thus includes various subsystems:

### Linear algebra library
Templated library for linear algebra operations, including vectors, matrices, and quaternions, as well as common numeric operations required for graphics programming.
### Platform abstraction layer
Platform specific operations like file handling, window creation, or input handling are abstracted into common interfaces, facilitating operation on desktop and android targets.
### Graphics pipeline abstraction layer
The graphics pipeline has its own abstraction layer with high-level interfaces for common graphics objects, like shaders, textures, and drawable vertices. The focus is to provide implementations for OpenGL for desktop and OpenGL-ES for android.
### Image module
Handles image file reading/writing and provides a common representation for image data on the CPU (as opposed to GPU textures). 
### 3D-Graphics module
Bundles graphics primitives into higher-order objects like meshes, materials, or lights and organizes them in a scene graph for hierarchical rendering.   
### 3D-Physics module ([goto](source/physics3d))
Implements the simulation of rigid bodies in 3D. Contains a force integrator, collision detection, and collision resolution through a constraint-based approach (Sequential Impulses). 
### Text rendering module
Uses signed distance fields to generate fonts in order to render unicode text at arbitrary sizes with a minimal memory footprint. Includes a generator for SDF fonts from true type fonts and a text renderer.
### GUI module
Provides a framework for building simple hierarchical GUIs with some preliminary automatic layout options like widget stacking or anchoring.  

## Demos
### Physics-based rendering:
TODO: add screenshots

### Physics simulation:
![New Project](https://github.com/NiklasReiche/yage/assets/29310846/33ae2170-e69c-4650-bbad-0cdec81dcc04)

### GUI showcase:
>![gui_demo_WolSdPGhhf](https://github.com/NiklasReiche/yage/assets/29310846/2a8c30e1-f8b3-4e37-9a8d-a6393054becd)

### Text renderer:
>![text_renderer_ZNWA41Qhvw](https://github.com/NiklasReiche/yage/assets/29310846/ea7f4650-f3eb-466a-896e-8f0691aa0f96)

## Dependencies
- Catch2 for writing unit tests
- Glad for the OpenGL headers
- Vulkan SDK for the Vulkan headers
- GLFW for window creation on desktop targets
- libpng for reading PNG images
- tinygltf for reading GlTF files
- FreeType for reading TrueType font files 

Except Glad and Vulkan, these dependencies are downloaded and configured automatically by the CMake build. 
