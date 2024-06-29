#include "shaders.h"

namespace yage::gl3d::shaders{
    const std::string Pbr::vert = R"(@PBR_VERT@)";
    const std::string Pbr::frag = R"(@PBR_FRAG@)";

    const std::string PbrNormalMapping::vert = R"(@PBR_NORMAL_VERT@)";
    const std::string PbrNormalMapping::frag = R"(@PBR_NORMAL_FRAG@)";

    const std::string Phong::vert = R"(@PHONG_VERT@)";
    const std::string Phong::frag = R"(@PHONG_FRAG@)";

    const std::string PhongNormalMapping::vert = R"(@PHONG_NORMAL_VERT@)";
    const std::string PhongNormalMapping::frag = R"(@PHONG_NORMAL_FRAG@)";

    const std::string Skybox::vert = R"(@SKYBOX_VERT@)";
    const std::string Skybox::frag = R"(@SKYBOX_FRAG@)";
}
