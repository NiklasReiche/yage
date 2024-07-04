#pragma once

#include <memory>

#include "../IDrawableCreator.h"

namespace yage::gl::vulkan
{
    class Instance;

    class DrawableCreator final : IDrawableCreator2
    {
    public:
        explicit DrawableCreator(std::weak_ptr<Instance> m_instance);

        [[nodiscard]] Handle<IDrawable2> create(VertexDataInfo vertex_data_info, std::span<const std::byte> vertices,
                                                IndexDataInfo index_data_info,
                                                std::span<const std::byte> indices) const override;

    private:
        std::weak_ptr<Instance> m_instance;
    };
}
