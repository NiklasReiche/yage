#pragma once

#include <stdexcept>
#include <vulkan/vulkan.h>

#include "../enums.h"

namespace yage::gl::vulkan
{
    inline VkPolygonMode convert(const PolygonMode polygon_mode)
    {
        switch (polygon_mode) {
            case PolygonMode::FILL:
                return VK_POLYGON_MODE_FILL;
            case PolygonMode::LINE:
                return VK_POLYGON_MODE_LINE;
            case PolygonMode::POINT:
                return VK_POLYGON_MODE_POINT;
            default:
                throw std::invalid_argument("Unknown PolygonMode value");
        }
    }

    inline VkCullModeFlagBits convert(const CullMode cull_mode)
    {
        switch (cull_mode) {
            case CullMode::NONE:
                return VK_CULL_MODE_NONE;
            case CullMode::FRONT_FACE:
                return VK_CULL_MODE_FRONT_BIT;
            case CullMode::BACK_FACE:
                return VK_CULL_MODE_BACK_BIT;
            case CullMode::FRONT_AND_BACK_FACE:
                return VK_CULL_MODE_FRONT_AND_BACK;
            default:
                throw std::invalid_argument("Unknown CullMode value");
        }
    }

    inline VkPrimitiveTopology convert(const PrimitiveTopology topology)
    {
        switch (topology) {
            case PrimitiveTopology::POINT_LIST:
                return VK_PRIMITIVE_TOPOLOGY_POINT_LIST;
            case PrimitiveTopology::LINE_LIST:
                return VK_PRIMITIVE_TOPOLOGY_LINE_LIST;
            case PrimitiveTopology::LINE_STRIP:
                return VK_PRIMITIVE_TOPOLOGY_LINE_STRIP;
            case PrimitiveTopology::TRIANGLE_LIST:
                return VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
            case PrimitiveTopology::TRIANGLE_STRIP:
                return VK_PRIMITIVE_TOPOLOGY_TRIANGLE_STRIP;
            default:
                throw std::invalid_argument("Unknown PrimitiveTopology value");
        }
    }

    inline VkFormat convert(const VertexComponent vertex_component)
    {
        switch (vertex_component.data_type) {
            case VertexComponentType::FLOAT_32: {
                switch (vertex_component.n_components) {
                    case VertexComponentSize::SCALAR:
                        return VK_FORMAT_R32_SFLOAT;
                    case VertexComponentSize::VEC2:
                        return VK_FORMAT_R32G32_SFLOAT;
                    case VertexComponentSize::VEC3:
                        return VK_FORMAT_R32G32B32_SFLOAT;
                    case VertexComponentSize::VEC4:
                        return VK_FORMAT_R32G32B32A32_SFLOAT;
                    default:
                        throw std::invalid_argument("Unknown VertexComponent combination");
                }
            }
            case VertexComponentType::FLOAT_64: {
                switch (vertex_component.n_components) {
                    case VertexComponentSize::SCALAR:
                        return VK_FORMAT_R64_SFLOAT;
                    case VertexComponentSize::VEC2:
                        return VK_FORMAT_R64G64_SFLOAT;
                    case VertexComponentSize::VEC3:
                        return VK_FORMAT_R64G64B64_SFLOAT;
                    case VertexComponentSize::VEC4:
                        return VK_FORMAT_R64G64B64A64_SFLOAT;
                    default:
                        throw std::invalid_argument("Unknown VertexComponent combination");
                }
            }
            case VertexComponentType::INT_16: {
                switch (vertex_component.n_components) {
                    case VertexComponentSize::SCALAR:
                        return VK_FORMAT_R16_SINT;
                    case VertexComponentSize::VEC2:
                        return VK_FORMAT_R16G16_SINT;
                    case VertexComponentSize::VEC3:
                        return VK_FORMAT_R16G16B16_SINT;
                    case VertexComponentSize::VEC4:
                        return VK_FORMAT_R16G16B16A16_SINT;
                    default:
                        throw std::invalid_argument("Unknown VertexComponent combination");
                }
            }
            case VertexComponentType::INT_32: {
                switch (vertex_component.n_components) {
                    case VertexComponentSize::SCALAR:
                        return VK_FORMAT_R32_SINT;
                    case VertexComponentSize::VEC2:
                        return VK_FORMAT_R32G32_SINT;
                    case VertexComponentSize::VEC3:
                        return VK_FORMAT_R32G32B32_SINT;
                    case VertexComponentSize::VEC4:
                        return VK_FORMAT_R32G32B32A32_SINT;
                    default:
                        throw std::invalid_argument("Unknown VertexComponent combination");
                }
            }
            case VertexComponentType::INT_64: {
                switch (vertex_component.n_components) {
                    case VertexComponentSize::SCALAR:
                        return VK_FORMAT_R64_SINT;
                    case VertexComponentSize::VEC2:
                        return VK_FORMAT_R64G64_SINT;
                    case VertexComponentSize::VEC3:
                        return VK_FORMAT_R64G64B64_SINT;
                    case VertexComponentSize::VEC4:
                        return VK_FORMAT_R64G64B64A64_SINT;
                    default:
                        throw std::invalid_argument("Unknown VertexComponent combination");
                }
            }
            case VertexComponentType::UINT_16: {
                switch (vertex_component.n_components) {
                    case VertexComponentSize::SCALAR:
                        return VK_FORMAT_R16_UINT;
                    case VertexComponentSize::VEC2:
                        return VK_FORMAT_R16G16_UINT;
                    case VertexComponentSize::VEC3:
                        return VK_FORMAT_R16G16B16_UINT;
                    case VertexComponentSize::VEC4:
                        return VK_FORMAT_R16G16B16A16_UINT;
                    default:
                        throw std::invalid_argument("Unknown VertexComponent combination");
                }
            }
            case VertexComponentType::UINT_32: {
                switch (vertex_component.n_components) {
                    case VertexComponentSize::SCALAR:
                        return VK_FORMAT_R32_UINT;
                    case VertexComponentSize::VEC2:
                        return VK_FORMAT_R32G32_UINT;
                    case VertexComponentSize::VEC3:
                        return VK_FORMAT_R32G32B32_UINT;
                    case VertexComponentSize::VEC4:
                        return VK_FORMAT_R32G32B32A32_UINT;
                    default:
                        throw std::invalid_argument("Unknown VertexComponent combination");
                }
            }
            case VertexComponentType::UINT_64: {
                switch (vertex_component.n_components) {
                    case VertexComponentSize::SCALAR:
                        return VK_FORMAT_R64_UINT;
                    case VertexComponentSize::VEC2:
                        return VK_FORMAT_R64G64_UINT;
                    case VertexComponentSize::VEC3:
                        return VK_FORMAT_R64G64B64_UINT;
                    case VertexComponentSize::VEC4:
                        return VK_FORMAT_R64G64B64A64_UINT;
                    default:
                        throw std::invalid_argument("Unknown VertexComponent combination");
                }
            }
            default:
                throw std::invalid_argument("Unknown VertexComponent combination");
        }
    }
}
