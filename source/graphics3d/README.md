# 3D-Graphics module
## Features
- Scene graph
- Forward renderer
- PBR shaders (with normal mapped and textured materials)
- Meshes/Submeshes/Materials

## Architecture

![uml_gl3d drawio](https://github.com/NiklasReiche/yage/assets/29310846/721f974d-ecd8-475e-a99d-a2b302ae40de)

## TODO
- Improve renderer interface (e.g. for controlling blending)
- Shadow mapping
- Deferred renderer
- Modular shader system for permutations
- Spotlights
- Frustum culling
- Render optimizations (minimize state changes and redundant gl-calls)