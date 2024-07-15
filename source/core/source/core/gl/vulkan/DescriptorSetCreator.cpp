#include "DescriptorSetCreator.h"
#include "Instance.h"

namespace yage::gl::vulkan
{
    DescriptorSetCreator::DescriptorSetCreator(std::weak_ptr<Instance> instance)
        : m_instance(std::move(instance))
    {
    }

    DescriptorSetHandle DescriptorSetCreator::create(DescriptorSetLayoutHandle layout, const ResourceUsage usage)
    {
        const auto instance = m_instance.lock();

        FrameCounter frame_counter = instance->frame_counter_for_usage(usage);

        const std::vector layouts(frame_counter.max_frame_index, layout.get<DescriptorSetLayout>().vk_handle());

        VkDescriptorSetAllocateInfo alloc_info{};
        alloc_info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
        alloc_info.descriptorPool = instance->descriptor_allocator().vk_pool();
        alloc_info.descriptorSetCount = frame_counter.max_frame_index;
        alloc_info.pSetLayouts = layouts.data();

        return instance->descriptor_allocator().store_descriptor_sets().create(instance.get(), alloc_info,
                                                                               frame_counter, std::move(layout));
    }
}
