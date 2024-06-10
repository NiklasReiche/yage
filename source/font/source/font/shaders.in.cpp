#include "shaders.h"

namespace yage::font::shaders{
    const std::string TextShader::vert = R"(@TEXT_VERT@)";
    const std::string TextShader::frag = R"(@TEXT_FRAG@)";
}
