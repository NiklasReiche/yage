#include "shaders.h"

namespace yage::gl3d::shaders{
    const std::string PbrNormalMappingShader::vert = R"(@PBR_NORMAL_VERT@)";
    const std::string PbrNormalMappingShader::frag = R"(@PBR_NORMAL_FRAG@)";

    const std::string PbrShader::vert = R"(@PBR_VERT@)";
    const std::string PbrShader::frag = R"(@PBR_FRAG@)";
}
