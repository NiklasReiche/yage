#include "shaders.h"

namespace yage::physics3d::shaders{
    const std::string PointShader::vert = R"(@EMPTY_VERT@)";
    const std::string PointShader::geom = R"(@POINT_GEOM@)";
    const std::string PointShader::frag = R"(@PASS_THROUGH_FRAG@)";

    const std::string VectorShader::vert = R"(@EMPTY_VERT@)";
    const std::string VectorShader::geom = R"(@VECTOR_GEOM@)";
    const std::string VectorShader::frag = R"(@PASS_THROUGH_FRAG@)";
}
