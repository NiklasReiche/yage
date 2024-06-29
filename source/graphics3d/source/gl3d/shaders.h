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
        PHONG,
        PHONG_NORMAL_MAP,
        SKYBOX
    };

    using ShaderMap = std::unordered_map<ShaderPermutation, std::shared_ptr<gl::IShader>>;

    namespace shaders
    {
        struct Pbr
        {
            static const std::string vert;
            static const std::string frag;
        };

        struct PbrNormalMapping
        {
            static const std::string vert;
            static const std::string frag;
        };

        struct Phong
        {
            static const std::string vert;
            static const std::string frag;
        };

        struct PhongNormalMapping
        {
            static const std::string vert;
            static const std::string frag;
        };

        struct Skybox
        {
            static const std::string vert;
            static const std::string frag;
        };
    }
}
