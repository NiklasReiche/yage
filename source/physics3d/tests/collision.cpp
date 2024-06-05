#include <catch2/catch_all.hpp>

#include <physics3d/BoundingShape.h>

using namespace physics3d;
using namespace gml;

TEST_CASE("RadDegConversion")
{
    BOrientedBox b1{
            .min = Vec3d(-1, 0, 0),
            .max = Vec3d(0.1, 1, 1),
    };

    BOrientedBox b2{
            .min = Vec3d(0, 0, 0),
            .max = Vec3d(1, 1, 1),
    };

    CollisionVisitor v;
    std::optional<ContactManifold> manifold = v(b1, b2);

    CHECK(manifold.has_value());
}
