#pragma once

#include <cassert>
#include <cstdint>
#include <numeric>
#include <span>
#include <stdexcept>
#include <vector>

#include "math/vector.h"

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
        SINT_16,
        SINT_32,
        SINT_64, // TODO: opengl does not support 64 bit integers without extensions
        UINT_16, // TODO: are unsigned components even possible in GLSL?
        UINT_32,
        UINT_64 // TODO: opengl does not support 64 bit integers without extensions
    };

    inline std::size_t byte_size(const VertexComponentType data_type)
    {
        switch (data_type) {
            case VertexComponentType::FLOAT_32: return 4;
            case VertexComponentType::FLOAT_64: return 8;
            case VertexComponentType::SINT_16:  return 2;
            case VertexComponentType::SINT_32:  return 4;
            case VertexComponentType::SINT_64:  return 8;
            case VertexComponentType::UINT_16:  return 2;
            case VertexComponentType::UINT_32:  return 4;
            case VertexComponentType::UINT_64:  return 8;

            default: throw std::invalid_argument("Unknown VertexComponentType value");
        }
    }

    enum class VertexComponentSize
    {
        SCALAR = 1,
        VEC2 = 2,
        VEC3 = 3,
        VEC4 = 4,
    };

    // TODO: we could combine VertexComponentSize and VertexComponentType into a single enum like with image formats
    struct VertexComponent
    {
        VertexComponentSize n_components;
        VertexComponentType data_type;
    };

    inline std::size_t byte_size(const VertexComponent vertex_element)
    {
        return static_cast<std::size_t>(vertex_element.n_components) * byte_size(vertex_element.data_type);
    }

    inline std::size_t byte_size(const std::span<const VertexComponent> vertex_description)
    {
        return std::accumulate(
                vertex_description.begin(), vertex_description.end(), static_cast<std::size_t>(0),
                [](const std::size_t acc, const VertexComponent vertex) { return acc + byte_size(vertex); });
    }

    enum class VertexDataLayout
    {
        INTERLEAVED,
        BATCHED
    };

    struct VertexDataInfo
    {
        std::vector<VertexComponent> vertex_description;
        VertexDataLayout data_layout;
    };

    inline std::size_t vertex_count(const VertexDataInfo& vertex_data_info,
                                    const std::span<const std::byte> vertex_data)
    {
        const std::size_t vertex_byte_size = byte_size(vertex_data_info.vertex_description);
        assert(vertex_data.size() % vertex_byte_size == 0);
        return vertex_data.size() / vertex_byte_size;
    }

    enum class IndexType
    {
        UINT_16 = 2,
        UINT_32 = 4,
    };

    inline std::size_t byte_size(const IndexType index_type)
    {
        return static_cast<std::size_t>(index_type);
    }

    struct IndexDataInfo
    {
        IndexType data_type;
        std::size_t index_count;
    };

    enum class TextureFormat2
    {
        UNDEFINED,
        // stored as 8-bit integers, interpreted as [0.0f, 1.0f] floats in linear space
        R8_UNORM,
        R8G8_UNORM,
        R8G8B8_UNORM,
        R8G8B8A8_UNORM,
        // stored as 8-bit integers, interpreted as [0.0f, 1.0f] floats in srgb space
        // we don't have R8_UNORM_SRGB or R8G8_UNORM_SRGB, since OpenGL doesn't support those
        R8G8B8_UNORM_SRGB,
        R8G8B8A8_UNORM_SRGB,
        // stored as 8-bit integers, interpreted as [-1.0f, 1.0f] floats in linear space
        R8_SNORM,
        R8G8_SNORM,
        R8G8B8_SNORM,
        R8G8B8A8_SNORM,
        // stored as 8-bit integers, interpreted as [0, 255] integers
        R8_UINT,
        R8G8_UINT,
        R8G8B8_UINT,
        R8G8B8A8_UINT,
        // stored as 8-bit integers, interpreted as [-128, 127] integers
        R8_SINT,
        R8G8_SINT,
        R8G8B8_SINT,
        R8G8B8A8_SINT,
        // stored as 16-bit integers, interpreted as [0.0f, 1.0f] floats in linear space
        R16_UNORM,
        R16G16_UNORM,
        R16G16B16_UNORM,
        R16G16B16A16_UNORM,
        // stored as 16-bit integers, interpreted as [-1.0f, 1.0f] floats in linear space
        R16_SNORM,
        R16G16_SNORM,
        R16G16B16_SNORM,
        R16G16B16A16_SNORM,
        // stored as 16-bit integers, interpreted as [0, 65535] integers
        R16_UINT,
        R16G16_UINT,
        R16G16B16_UINT,
        R16G16B16A16_UINT,
        // stored as 16-bit integers, interpreted as [-32768, 32767] integers
        R16_SINT,
        R16G16_SINT,
        R16G16B16_SINT,
        R16G16B16A16_SINT,
    };

    struct PixelTransferInfo
    {
        TextureFormat2 image_format;
        // TODO: opengl has signed 32-bit, vulkan has unsigned 32-bit texture sizes
        std::uint32_t width; // TODO: use signed
        std::uint32_t height;
    };

    struct Viewport // TODO: opengl does not have float viewports, so we could merge this with the scissor rect
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
        unsigned int width = 0; // TODO: signed type
        unsigned int height = 0;
    };

    enum class ResourceUsage
    {
        // TODO: add specification for read/write/...
        STATIC,
        DYNAMIC
    };

    enum class TextureWrapper2
    {
        CLAMP_TO_EDGE,
        CLAMP_TO_BORDER,
        REPEAT,
        MIRRORED_REPEAT
    };

    enum class TextureFilter2
    {
        NEAREST,
        LINEAR
    };

    enum class MipMapMode
    {
        NEAREST,
        LINEAR,
        NONE
    };

    struct TextureSampler
    {
        TextureFilter2 min_filter = TextureFilter2::LINEAR;
        TextureFilter2 mag_filter = TextureFilter2::LINEAR;

        MipMapMode mip_map_mode = MipMapMode::NONE;

        TextureWrapper2 u_wrapper = TextureWrapper2::REPEAT;
        TextureWrapper2 v_wrapper = TextureWrapper2::REPEAT;
        TextureWrapper2 w_wrapper = TextureWrapper2::REPEAT;

        // TODO: border color
    };

    enum class MSAASamples
    {
        SAMPLE_1,
        SAMPLE_2,
        SAMPLE_4,
        SAMPLE_8
    };

    inline unsigned int n_samples(const MSAASamples samples)
    {
        switch (samples) {
            case MSAASamples::SAMPLE_1: return 1;
            case MSAASamples::SAMPLE_2: return 2;
            case MSAASamples::SAMPLE_4: return 4;
            case MSAASamples::SAMPLE_8: return 8;

            default: throw std::invalid_argument("unknown MSSASamples value");
        }
    }

    enum class BlendFactor
    {
        ZERO,
        ONE,
        SRC_COLOR,
        ONE_MINUS_SRC_COLOR,
        DST_COLOR,
        ONE_MINUS_DST_COLOR,
        SRC_ALPHA,
        ONE_MINUS_SRC_ALPHA,
        DST_ALPHA,
        ONE_MINUS_DST_ALPHA,
        CONSTANT_COLOR,
        ONE_MINUS_CONSTANT_COLOR,
        CONSTANT_ALPHA,
        ONE_MINUS_CONSTANT_ALPHA
    };

    enum class BlendOperation
    {
        ADD,
        SUBTRACT,
        REVERSE_SUBTRACT,
        MIN,
        MAX
    };

    struct BlendInfo
    {
        BlendFactor source_factor = BlendFactor::SRC_ALPHA;
        BlendFactor destination_factor = BlendFactor::ONE_MINUS_SRC_ALPHA;
        BlendOperation operation = BlendOperation::ADD;
        math::Vec4f constant;
    };
}
