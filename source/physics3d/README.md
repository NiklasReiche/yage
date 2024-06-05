# 3D-Physics module
## Features

- Rigid body dynamics in 3D
- Implicit euler integrator (force-based movement)
- Collision detection (spheres, planes)
- Iterative constraint solver for collision resolution (Sequential Impulses method)

## Architecture

![uml_diagram](https://github.com/NiklasReiche/yage/assets/29310846/1fac27cd-31d2-4f97-87ac-e9ab707d1a0b)

## TODO

- General calculation optimizations (simplify formulas, reuse results)
- Collision detection for convex polyhedrons (SAT, GJK)
- Broad phase for collision detection (Octree, BVH)
- Different approaches for position correction (Split Impulse)
- Warm starting for the constraint solver
- Continuous collision detection
