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
}
