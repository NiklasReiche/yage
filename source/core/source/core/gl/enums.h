#pragma once

#include <cassert>
#include <cstdint>
#include <numeric>
#include <span>
#include <stdexcept>
#include <vector>

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
        SINT_64,
        UINT_16, // TODO: are unsigned components even possible in GLSL?
        UINT_32,
        UINT_64
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

    enum class ImageFormat2
    {
        R8_UNORM, // linear space format, values in range [0.0f, 255.0f]
        R8G8_UNORM,
        R8G8B8_UNORM,
        R8G8B8A8_UNORM,
        R8_UNORM_SRGB, // srgb space format, values in range [0.0f, 255.0f]
        R8G8_UNORM_SRGB,
        R8G8B8_UNORM_SRGB,
        R8G8B8A8_UNORM_SRGB,
        R8_SNORM, // values in range [-1.0f, 1.0f]
        R8G8_SNORM,
        R8G8B8_SNORM,
        R8G8B8A8_SNORM,
        R8_UINT, // values in range [0, 255]
        R8G8_UINT,
        R8G8B8_UINT,
        R8G8B8A8_UINT,
        R8_SINT, // values in range [-128, 127]
        R8G8_SINT,
        R8G8B8_SINT,
        R8G8B8A8_SINT,
    };

    struct PixelTransferInfo
    {
        ImageFormat2 image_format;
        std::uint32_t width;
        std::uint32_t height;
    };

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

    enum class ResourceUsage
    {
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
}
