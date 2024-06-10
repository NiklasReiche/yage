# 3D-Physics module
## Features

- Rigid body dynamics in 3D
- Implicit euler integrator (force-based movement)
- Collision detection (spheres, planes, oriented boxes)
- Iterative constraint solver for collision resolution (Sequential Impulses method)

## Architecture

![uml_diagram](https://github.com/NiklasReiche/yage/assets/29310846/f460c297-3714-4313-b21c-1cd876ebbb63)

## TODO

- General calculation optimizations (simplify formulas, reuse results)
- Collision detection for convex polyhedra (GJK)
- Broad phase for collision detection (Octree, BVH)
- Different approaches for position correction (Split Impulse)
- Warm starting for the constraint solver
- Continuous collision detection
