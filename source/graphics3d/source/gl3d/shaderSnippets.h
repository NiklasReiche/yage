#pragma once

#include <string>

namespace gl3d
{
    struct ShaderSnippets
    {
        constexpr static std::string LIGHT_COLOR_NAME = "color";

        constexpr static std::string POINT_LIGHTS_AMOUNT_NAME = "n_pointLights";
        constexpr static std::string POINT_LIGHTS_NAME = "pointLights";
        constexpr static std::string POINT_LIGHT_POSITION_NAME = "position";

        constexpr static std::string DIR_LIGHTS_AMOUNT_NAME = "n_dirLights";
        constexpr static std::string DIR_LIGHTS_NAME = "dirLights";
        constexpr static std::string DIR_LIGHT_DIRECTION_NAME = "direction";
    };
}
