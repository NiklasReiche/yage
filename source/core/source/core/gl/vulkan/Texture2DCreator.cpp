#include "Texture2DCreator.h"

#include "Instance.h"

namespace yage::gl::vulkan
{
    Texture2DCreator::Texture2DCreator(std::weak_ptr<Instance> instance)
        : m_instance(std::move(instance))
    {
    }

    Handle<ITexture2D2> Texture2DCreator::create(const PixelTransferInfo& data_info,
                                                 const std::span<const std::byte> data) const
    {
        return m_instance.lock()->store_textures()->create(m_instance, data_info, data);
    }
}
