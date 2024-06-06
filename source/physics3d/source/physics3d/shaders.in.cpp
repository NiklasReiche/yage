#include "shaders.h"

namespace physics3d::shaders{
    const std::string PointShader::vert = R"(@POINT_VERT@)";
    const std::string PointShader::geom = R"(@POINT_GEOM@)";
    const std::string PointShader::frag = R"(@POINT_FRAG@)";
}
