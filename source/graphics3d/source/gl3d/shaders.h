#pragma once

#include <string>
#include <unordered_map>
#include <memory>

#include <core/gl/Shader.h>

namespace yage::gl3d
{
    enum class ShaderPermutation
    {
        PBR,
        PBR_NORMAL_MAP,
        SKYBOX
    };

    using ShaderMap = std::unordered_map<ShaderPermutation, std::shared_ptr<gl::IShader>>;

    namespace shaders
    {
        struct PbrNormalMappingShader
        {
            static const std::string vert;
            static const std::string frag;

            static std::string get_vert();

            static std::string get_frag();
        };

        struct PbrShader
        {
            static const std::string vert;
            static const std::string frag;

            static std::string get_vert();

            static std::string get_frag();
        };

        struct SkyboxShader
        {
            static const std::string vert;
            static const std::string frag;
        };
    }
}
