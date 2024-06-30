#pragma once

#include <stdexcept>

namespace yage::gl
{
    enum class PolygonMode
    {
        FILL,
        LINE,
        POINT
    };

    enum class CullMode
    {
        NONE,
        FRONT_FACE,
        BACK_FACE,
        FRONT_AND_BACK_FACE
    };

    enum class PrimitiveTopology
    {
        POINT_LIST,
        LINE_LIST,
        LINE_STRIP,
        TRIANGLE_LIST,
        TRIANGLE_STRIP
    };

    enum class VertexComponentType
    {
        FLOAT_32,
        FLOAT_64,
        INT_16,
        INT_32,
        INT_64,
        UINT_16,
        UINT_32,
        UINT_64
    };

    inline std::size_t byte_size(const VertexComponentType data_type)
    {
        switch (data_type) {
            case VertexComponentType::FLOAT_32:
                return 4;
            case VertexComponentType::FLOAT_64:
                return 8;
            case VertexComponentType::INT_16:
                return 2;
            case VertexComponentType::INT_32:
                return 4;
            case VertexComponentType::INT_64:
                return 8;
            case VertexComponentType::UINT_16:
                return 2;
            case VertexComponentType::UINT_32:
                return 4;
            case VertexComponentType::UINT_64:
                return 8;
            default:
                throw std::invalid_argument("Unknown VertexDataType value");
        }
    }

    enum class VertexComponentSize
    {
        SCALAR = 1,
        VEC2 = 2,
        VEC3 = 3,
        VEC4 = 4,
    };

    struct VertexComponent
    {
        VertexComponentSize n_components;
        VertexComponentType data_type;
    };

    inline std::size_t byte_size(const VertexComponent vertex_element)
    {
        return static_cast<std::size_t>(vertex_element.n_components) * byte_size(vertex_element.data_type);
    }

    struct Viewport
    {
        float x = 0;
        float y = 0;
        float width = 0;
        float height = 0;
    };

    struct ScissorRectangle
    {
        int x = 0;
        int y = 0;
        unsigned int width = 0;
        unsigned int height = 0;
    };
}
