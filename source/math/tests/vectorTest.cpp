#include <catch2/catch_all.hpp>

#include <math/maths.h>
#include <math/vector.h>

#include "./complex.h"

using namespace yage::math;

TEST_CASE("VectorBase test")
{
    SECTION("Constructor") {
        const std::size_t size = 5;

        SECTION("Default with Primitives") {
            Vector<int, size> vector;

            for (std::size_t i = 0; i < size; ++i) {
                CHECK(0 == vector(i));
            }
        }

        SECTION("Default with Classes") {
            Vector<ComplexD, size> complexVector;

            for (std::size_t i = 0; i < size; ++i) {
                CHECK(0 == complexVector(i).real);
                CHECK(0 == complexVector(i).imaginary);
            }
        }

        SECTION("Fill") {
            Vector<int, size> vector(4);

            for (std::size_t i = 0; i < size; ++i) {
                CHECK(4 == vector(i));
            }
        }

        SECTION("variadic argument list") {
            Vector<float, 6> vector(2.0f, -1.5f, 0.0f, 1.f, 2.f, 3.f);
            CHECK(2.0f == vector(0));
            CHECK(-1.5f == vector(1));
            CHECK(0.0f == vector(2));
            CHECK(1.0f == vector(3));
            CHECK(2.0f == vector(4));
            CHECK(3.0f == vector(5));
        }

        SECTION("span") {
            std::array data{2.0f, -1.5f, 0.0f, 1.f, 2.f, 3.f};
            Vector<float, 6> vector(data);
            CHECK(2.0f == vector(0));
            CHECK(-1.5f == vector(1));
            CHECK(0.0f == vector(2));
            CHECK(1.0f == vector(3));
            CHECK(2.0f == vector(4));
            CHECK(3.0f == vector(5));
        }

        SECTION("Copy") {
            Vector<int, 2> initial{2, 5};

            Vector<int, 2> copy = initial;

            CHECK(2 == initial(0));
            CHECK(5 == initial(1));
            CHECK(2 == copy(0));
            CHECK(5 == copy(1));
        }

        SECTION("Conversion with primitive") {
            Vector<int, 2> initial{2, 5};

            Vector<float, 2> copy = static_cast<Vector<float, 2>>(initial); // NOLINT(*-use-auto)

            CHECK(2 == initial(0));
            CHECK(5 == initial(1));
            CHECK(Catch::Approx(2) == copy(0));
            CHECK(Catch::Approx(5) == copy(1));
        }

        SECTION("Conversion with classes") {
            Vector<ComplexD, 2> initial{
                {2, 5},
                {1, 8}
            };

            Vector<ComplexF, 2> copy = static_cast<Vector<ComplexF, 2>>(initial); // NOLINT(*-use-auto)

            CHECK(Catch::Approx(2) == initial(0).real);
            CHECK(Catch::Approx(5) == initial(0).imaginary);
            CHECK(Catch::Approx(1) == initial(1).real);
            CHECK(Catch::Approx(8) == initial(1).imaginary);

            CHECK(Catch::Approx(2) == copy(0).real);
            CHECK(Catch::Approx(5) == copy(0).imaginary);
            CHECK(Catch::Approx(1) == copy(1).real);
            CHECK(Catch::Approx(8) == copy(1).imaginary);
        }

        SECTION("explicit 2-dimensional") {
            Vec2<int> vec(25, 42);

            CHECK(25 == vec.x());
            CHECK(42 == vec.y());
        }

        SECTION("explicit 3-dimensional") {
            Vec3<int> vec(25, 42, 63);

            CHECK(25 == vec.x());
            CHECK(42 == vec.y());
            CHECK(63 == vec.z());
        }

        SECTION("explicit 4-dimensional") {
            Vec4<int> vec(25, 42, 63, 10);

            CHECK(25 == vec.x());
            CHECK(42 == vec.y());
            CHECK(63 == vec.z());
            CHECK(10 == vec.w());
        }
    }

    SECTION("Assignment") {
        SECTION("Assignment_Copy") {
            Vector<int, 2> initial{2, 5};
            Vector<int, 2> copy{0, 0};

            copy = initial;

            CHECK(2 == initial(0));
            CHECK(5 == initial(1));
            CHECK(2 == copy(0));
            CHECK(5 == copy(1));
        }

        SECTION("Assignment_Conversion_WithPrimitive") {
            Vector<int, 2> initial{2, 5};
            Vector<float, 2> copy;

            copy = static_cast<Vector<float, 2>>(initial);

            CHECK(2 == initial(0));
            CHECK(5 == initial(1));
            CHECK(Catch::Approx(2) == copy(0));
            CHECK(Catch::Approx(5) == copy(1));
        }

        SECTION("Assignment_Conversion_WithClasses") {
            Vector<ComplexD, 2> initial{
                {2, 5},
                {1, 8}
            };
            Vector<ComplexF, 2> copy;

            copy = static_cast<Vector<ComplexF, 2>>(initial);

            CHECK(Catch::Approx(2) == initial(0).real);
            CHECK(Catch::Approx(5) == initial(0).imaginary);
            CHECK(Catch::Approx(1) == initial(1).real);
            CHECK(Catch::Approx(8) == initial(1).imaginary);

            CHECK(Catch::Approx(2) == copy(0).real);
            CHECK(Catch::Approx(5) == copy(0).imaginary);
            CHECK(Catch::Approx(1) == copy(1).real);
            CHECK(Catch::Approx(8) == copy(1).imaginary);
        }
    }

    SECTION("Operators") {
        SECTION("Equality") {
            CHECK(Vec3i(3, 2, 4) == Vec3i(3, 2, 4));
            CHECK(!(Vec3i(3, 1, 4) == Vec3i(3, 2, 4)));
            CHECK(Vec3i( 3, 1, 4 ) != Vec3i(3, 2, 4 ));
            CHECK(!(Vec3i( 3, 2, 4 ) != Vec3i( 3, 2, 4 )));
        }

        SECTION("Add") {
            const Vec3i vec1(2, 1, 3);
            const Vec3i vec2(2, -4, 0);

            Vec3i expected(4, -3, 3);
            Vec3i result;

            result = vec1 + vec2;
            CHECK(expected == result);

            result = vec2 + vec1;
            CHECK(expected == result);

            result = vec1;
            result += vec2;
            CHECK(expected == result);
        }

        SECTION("Subtract") {
            const Vec3i vec1(2, 1, 3);
            const Vec3i vec2(2, -4, 0);

            Vec3i expected(0, 5, 3);
            Vec3i result;

            result = vec1 - vec2;
            CHECK(expected == result);

            result = vec2 - vec1;
            expected = Vec3i(0, -5, -3);
            CHECK(expected == result);

            result = vec2;
            result -= vec1;
            CHECK(expected == result);

            CHECK(Vec3i(-1, -3, 2) == -Vec3i(1, 3, -2));
        }

        SECTION("Multiply") {
            const Vec3i vec1(2, 1, 3);
            const int factor = 3;

            Vec3i expected(6, 3, 9);
            Vec3i result;

            result = vec1 * factor;
            CHECK(expected == result);

            result = factor * vec1;
            CHECK(expected == result);

            result = vec1;
            result *= factor;
            CHECK(expected == result);
        }

        SECTION("division") {
            Vec3d vec(2, 1, 3);

            CHECK(Vec3d(1, 0.5, 1.5) == vec / 2.);

            SECTION("division assignment") {
                vec /= 2;
                CHECK(Vec3d(1, 0.5, 1.5) == vec);
            }
        }
    }

    SECTION("Dot") {
        const Vec3d vec1(2, 1, 3);
        const Vec3d vec2(-2, 0, 1);

        CHECK(-1.0 == dot(vec1, vec2));
    }

    SECTION("Normalize") {
        Vec3d vec1(2, 1, 3);

        Vec3d expected(2 / std::sqrt(14), 1 / std::sqrt(14), 3 / std::sqrt(14));

        CHECK(Catch::Approx(std::sqrt(14)) == length(vec1));

        Vec3d result = normalize(vec1);
        CHECK(Catch::Approx(expected.x()) == result.x());
        CHECK(Catch::Approx(expected.y()) == result.y());
        CHECK(Catch::Approx(expected.z()) == result.z());

        vec1.normalize();
        CHECK(Catch::Approx(expected.x()) == vec1.x());
        CHECK(Catch::Approx(expected.y()) == vec1.y());
        CHECK(Catch::Approx(expected.z()) == vec1.z());
    }

    SECTION("Length") {
        const Vec3d vec1(2, 1, 3);

        CHECK(Catch::Approx(14) == length_sqr(vec1));
        CHECK(Catch::Approx(std::sqrt(14)) == length(vec1));
    }

    SECTION("Cross") {
        const Vec3d vec1(2, 1, 3);
        const Vec3d vec2(-1, 5, 1);

        CHECK(Vec3d(-14, -5, 11) == cross(vec1, vec2));
    }

    SECTION("angle") {
        SECTION("Angle_2D") {
            const Vec2d vec1(1, 0);
            const Vec2d vec2(1, 1);

            CHECK(Catch::Approx(to_rad(45.0)) == angle_rad(vec1, vec2));
        }

        SECTION("Angle_3D") {
            const Vec3d vec1(1, 0, 0);
            const Vec3d vec2(1, 1, 0);

            CHECK(Catch::Approx(to_rad(45.0)) == angle_rad(vec1, vec2));
        }
    }
}
