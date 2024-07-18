#pragma once

#include <glad/gl.h>

#include "../enums.h"

namespace yage::gl::opengl4
{
    inline GLenum convert(const ResourceUsage usage)
    {
        switch (usage) {
            case ResourceUsage::STATIC:  return GL_STATIC_DRAW;
            case ResourceUsage::DYNAMIC: return GL_DYNAMIC_DRAW;

            default: throw std::invalid_argument("unknown RessourceUsage value");
        }
    }

    inline GLenum convert(const VertexComponentType component_type)
    {
        switch (component_type) {
            case VertexComponentType::SINT_16:  return GL_SHORT;
            case VertexComponentType::SINT_32:  return GL_INT;
            case VertexComponentType::UINT_16:  return GL_UNSIGNED_SHORT;
            case VertexComponentType::UINT_32:  return GL_UNSIGNED_INT;
            case VertexComponentType::FLOAT_32: return GL_FLOAT;
            case VertexComponentType::FLOAT_64: return GL_DOUBLE;

            default: throw std::invalid_argument("unknonw VertexComponentType value");
        }
    }
}
