#include "shaders.h"

namespace gl3d::shaders{
    const std::string PbrShader::vert = R"(@PBR_VERT@)";
    const std::string PbrShader::frag = R"(@PBR_FRAG@)";
}
