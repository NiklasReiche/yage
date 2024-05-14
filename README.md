# yage - Yet Another Game Engine
Small game engine / Work-in-progress

## Overview

## Demos

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
### 3D-graphics module
Bundles graphics primitives into higher-order objects like meshes, materials, or lights and organizes them in a scene graph for hierarchical rendering.   
### 3D-Physics module (early stages)
Bundles rigid body objects and simulates application of forces. The next steps include adding collision detection and octrees.  
### Text rendering module
Uses signed distance fields to generate fonts in order to render unicode text at arbitrary sizes with a minimal memory footprint. Includes a generator for SDF fonts from true type fonts and a text renderer.
### GUI module
Provides a framework for building simple hierarchical GUIs with some preliminary automatic layout options like widget stacking or anchoring.  
