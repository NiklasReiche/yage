# 3D-Physics module
## Features

- Rigid body dynamics in 3D
- Implicit euler integrator (force-based movement)
- Collision detection (spheres, planes)
- Iterative constraint solver for collision resolution (Sequential Impulses method)

## TODO

- General calculation optimizations (simplify formulas, reuse results)
- Collision detection for convex polyhedrons (SAT, GJK)
- Broad phase for collision detection (Octree, BVH)
- Different approaches for position correction (Split Impulse)
- Warm starting for the constraint solver