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

    inline GLenum convert(const IndexType type)
    {
        switch (type) {
            case IndexType::UINT_16: return GL_UNSIGNED_SHORT;
            case IndexType::UINT_32: return GL_UNSIGNED_INT;

            default: throw std::invalid_argument("unknonw IndexType value");
        }
    }

    inline GLenum to_internal_format(const TextureFormat2 format)
    {
        switch (format) {
            case TextureFormat2::R8_UNORM:            return GL_R8;
            case TextureFormat2::R8G8_UNORM:          return GL_RG8;
            case TextureFormat2::R8G8B8_UNORM:        return GL_RGB8;
            case TextureFormat2::R8G8B8A8_UNORM:      return GL_RGBA8;
            case TextureFormat2::R8G8B8_UNORM_SRGB:   return GL_SRGB8;
            case TextureFormat2::R8G8B8A8_UNORM_SRGB: return GL_SRGB8_ALPHA8;
            case TextureFormat2::R8_SNORM:            return GL_R8_SNORM;
            case TextureFormat2::R8G8_SNORM:          return GL_RG8_SNORM;
            case TextureFormat2::R8G8B8_SNORM:        return GL_RGB8_SNORM;
            case TextureFormat2::R8G8B8A8_SNORM:      return GL_RGBA8_SNORM;
            case TextureFormat2::R8_UINT:             return GL_R8UI;
            case TextureFormat2::R8G8_UINT:           return GL_RG8UI;
            case TextureFormat2::R8G8B8_UINT:         return GL_RGB8UI;
            case TextureFormat2::R8G8B8A8_UINT:       return GL_RGBA8UI;
            case TextureFormat2::R8_SINT:             return GL_R8I;
            case TextureFormat2::R8G8_SINT:           return GL_RG8I;
            case TextureFormat2::R8G8B8_SINT:         return GL_RGB8I;
            case TextureFormat2::R8G8B8A8_SINT:       return GL_RGBA8I;
            case TextureFormat2::R16_UNORM:           return GL_R16;
            case TextureFormat2::R16G16_UNORM:        return GL_RG16;
            case TextureFormat2::R16G16B16_UNORM:     return GL_RGB16;
            case TextureFormat2::R16G16B16A16_UNORM:  return GL_RGBA16;
            case TextureFormat2::R16_SNORM:           return GL_R16_SNORM;
            case TextureFormat2::R16G16_SNORM:        return GL_RG16_SNORM;
            case TextureFormat2::R16G16B16_SNORM:     return GL_RGB16_SNORM;
            case TextureFormat2::R16G16B16A16_SNORM:  return GL_RGBA16_SNORM;
            case TextureFormat2::R16_UINT:            return GL_R16UI;
            case TextureFormat2::R16G16_UINT:         return GL_RG16UI;
            case TextureFormat2::R16G16B16_UINT:      return GL_RGB16UI;
            case TextureFormat2::R16G16B16A16_UINT:   return GL_RGBA16UI;
            case TextureFormat2::R16_SINT:            return GL_R16I;
            case TextureFormat2::R16G16_SINT:         return GL_RG16I;
            case TextureFormat2::R16G16B16_SINT:      return GL_RGB16I;
            case TextureFormat2::R16G16B16A16_SINT:   return GL_RGBA16I;

            default: throw std::invalid_argument("Unknown ImageFormat value");
        }
    }

    inline GLenum to_client_format(const TextureFormat2 format)
    {
        switch (format) {
            case TextureFormat2::R8_UNORM:            return GL_RED;
            case TextureFormat2::R8G8_UNORM:          return GL_RG;
            case TextureFormat2::R8G8B8_UNORM:        return GL_RGB;
            case TextureFormat2::R8G8B8A8_UNORM:      return GL_RGBA;
            case TextureFormat2::R8G8B8_UNORM_SRGB:   return GL_RGB;
            case TextureFormat2::R8G8B8A8_UNORM_SRGB: return GL_RGBA;
            case TextureFormat2::R8_SNORM:            return GL_RED;
            case TextureFormat2::R8G8_SNORM:          return GL_RG;
            case TextureFormat2::R8G8B8_SNORM:        return GL_RGB;
            case TextureFormat2::R8G8B8A8_SNORM:      return GL_RGBA;
            case TextureFormat2::R8_UINT:             return GL_RED_INTEGER;
            case TextureFormat2::R8G8_UINT:           return GL_RG_INTEGER;
            case TextureFormat2::R8G8B8_UINT:         return GL_RGB_INTEGER;
            case TextureFormat2::R8G8B8A8_UINT:       return GL_RGBA_INTEGER;
            case TextureFormat2::R8_SINT:             return GL_RED_INTEGER;
            case TextureFormat2::R8G8_SINT:           return GL_RG_INTEGER;
            case TextureFormat2::R8G8B8_SINT:         return GL_RGB_INTEGER;
            case TextureFormat2::R8G8B8A8_SINT:       return GL_RGBA_INTEGER;
            case TextureFormat2::R16_UNORM:           return GL_RED;
            case TextureFormat2::R16G16_UNORM:        return GL_RG;
            case TextureFormat2::R16G16B16_UNORM:     return GL_RGB;
            case TextureFormat2::R16G16B16A16_UNORM:  return GL_RGBA;
            case TextureFormat2::R16_SNORM:           return GL_RED;
            case TextureFormat2::R16G16_SNORM:        return GL_RG;
            case TextureFormat2::R16G16B16_SNORM:     return GL_RGB;
            case TextureFormat2::R16G16B16A16_SNORM:  return GL_RGBA;
            case TextureFormat2::R16_UINT:            return GL_RED_INTEGER;
            case TextureFormat2::R16G16_UINT:         return GL_RG_INTEGER;
            case TextureFormat2::R16G16B16_UINT:      return GL_RGB_INTEGER;
            case TextureFormat2::R16G16B16A16_UINT:   return GL_RGBA_INTEGER;
            case TextureFormat2::R16_SINT:            return GL_RED_INTEGER;
            case TextureFormat2::R16G16_SINT:         return GL_RG_INTEGER;
            case TextureFormat2::R16G16B16_SINT:      return GL_RGB_INTEGER;
            case TextureFormat2::R16G16B16A16_SINT:   return GL_RGBA_INTEGER;

            default: throw std::invalid_argument("Unknown ImageFormat value");
        }
    }

    inline GLenum to_client_pixel_type(const TextureFormat2 format)
    {
        switch (format) {
            case TextureFormat2::R8_UNORM:
            case TextureFormat2::R8G8_UNORM:
            case TextureFormat2::R8G8B8_UNORM:
            case TextureFormat2::R8G8B8A8_UNORM:
            case TextureFormat2::R8G8B8_UNORM_SRGB:
            case TextureFormat2::R8G8B8A8_UNORM_SRGB:
            case TextureFormat2::R8_SNORM:
            case TextureFormat2::R8G8_SNORM:
            case TextureFormat2::R8G8B8_SNORM:
            case TextureFormat2::R8G8B8A8_SNORM:
            case TextureFormat2::R8_UINT:
            case TextureFormat2::R8G8_UINT:
            case TextureFormat2::R8G8B8_UINT:
            case TextureFormat2::R8G8B8A8_UINT:
            case TextureFormat2::R8_SINT:
            case TextureFormat2::R8G8_SINT:
            case TextureFormat2::R8G8B8_SINT:
            case TextureFormat2::R8G8B8A8_SINT:       return GL_UNSIGNED_BYTE;
            case TextureFormat2::R16_UNORM:
            case TextureFormat2::R16G16_UNORM:
            case TextureFormat2::R16G16B16_UNORM:
            case TextureFormat2::R16G16B16A16_UNORM:
            case TextureFormat2::R16_SNORM:
            case TextureFormat2::R16G16_SNORM:
            case TextureFormat2::R16G16B16_SNORM:
            case TextureFormat2::R16G16B16A16_SNORM:
            case TextureFormat2::R16_UINT:
            case TextureFormat2::R16G16_UINT:
            case TextureFormat2::R16G16B16_UINT:
            case TextureFormat2::R16G16B16A16_UINT:
            case TextureFormat2::R16_SINT:
            case TextureFormat2::R16G16_SINT:
            case TextureFormat2::R16G16B16_SINT:
            case TextureFormat2::R16G16B16A16_SINT:   return GL_UNSIGNED_SHORT;

            default: throw std::invalid_argument("Unknown ImageFormat value");
        }
    }

    inline GLenum convert(const TextureWrapper2 wrapper)
    {
        switch (wrapper) {
            case TextureWrapper2::CLAMP_TO_EDGE:   return GL_CLAMP_TO_EDGE;
            case TextureWrapper2::CLAMP_TO_BORDER: return GL_CLAMP_TO_BORDER;
            case TextureWrapper2::REPEAT:          return GL_REPEAT;
            case TextureWrapper2::MIRRORED_REPEAT: return GL_MIRRORED_REPEAT;

            default: throw std::invalid_argument("Unknown TextureWrapper value");
        }
    }

    inline GLenum convert(const TextureFilter2 filter, const MipMapMode mip_map_mode)
    {
        switch (mip_map_mode) {
            case MipMapMode::NONE:
                switch (filter) {
                    case TextureFilter2::LINEAR:  return GL_LINEAR;
                    case TextureFilter2::NEAREST: return GL_NEAREST;

                    default: throw std::invalid_argument("Unknown TextureFilter2 value");
                }

            case MipMapMode::LINEAR:
                switch (filter) {
                    case TextureFilter2::LINEAR:  return GL_LINEAR_MIPMAP_LINEAR;
                    case TextureFilter2::NEAREST: return GL_NEAREST_MIPMAP_LINEAR;

                    default: throw std::invalid_argument("Unknown TextureFilter2 value");
                }

            case MipMapMode::NEAREST:
                switch (filter) {
                    case TextureFilter2::LINEAR:  return GL_LINEAR_MIPMAP_NEAREST;
                    case TextureFilter2::NEAREST: return GL_NEAREST_MIPMAP_NEAREST;

                    default: throw std::invalid_argument("Unknown TextureFilter2 value");
                }

            default: throw std::invalid_argument("Unknown MipMapMode value");
        }
    }

    inline GLenum convert(const PrimitiveTopology topology)
    {
        switch (topology) {
            case PrimitiveTopology::POINT_LIST:     return GL_POINTS;
            case PrimitiveTopology::LINE_LIST:      return GL_LINES;
            case PrimitiveTopology::LINE_STRIP:     return GL_LINE_STRIP;
            case PrimitiveTopology::TRIANGLE_LIST:  return GL_TRIANGLES;
            case PrimitiveTopology::TRIANGLE_STRIP: return GL_TRIANGLE_STRIP;

            default: throw std::invalid_argument("Unknown PrimitiveTopology value");
        }
    }

    inline GLenum convert(const PolygonMode polygon_mode)
    {
        switch (polygon_mode) {
            case PolygonMode::POINT: return GL_POINT;
            case PolygonMode::LINE:  return GL_LINE;
            case PolygonMode::FILL:  return GL_FILL;

            default: throw std::invalid_argument("Unknown PolygonMode value");
        }
    }

    inline GLenum convert(const CullMode cull_mode)
    {
        switch (cull_mode) {
            case CullMode::FRONT_FACE:          return GL_FRONT;
            case CullMode::BACK_FACE:           return GL_BACK;
            case CullMode::FRONT_AND_BACK_FACE: return GL_FRONT_AND_BACK;

            default: throw std::invalid_argument("Unknown CullMode value");
        }
    }
}
