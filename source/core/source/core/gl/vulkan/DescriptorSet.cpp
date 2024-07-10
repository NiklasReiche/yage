#include "DescriptorSet.h"
#include "Instance.h"

namespace yage::gl::vulkan
{
    DescriptorSet::DescriptorSet(Instance* instance, const VkDescriptorSetAllocateInfo& alloc_info,
                                 const FrameCounter frame_counter, std::shared_ptr<Handle<DescriptorSetLayout>> layout)
        : m_instance(instance),
          m_vk_device(instance->device()),
          m_frame_counter(frame_counter),
          m_layout(std::move(layout))
    {
        m_vk_handles.resize(m_frame_counter.max_frame_index);

        if (vkAllocateDescriptorSets(m_vk_device, &alloc_info, m_vk_handles.data()) != VK_SUCCESS) {
            throw std::runtime_error("Vulkan: failed to allocate descriptor sets!");
        }
    }

    void DescriptorSet::write(const std::uint32_t binding, const std::shared_ptr<Handle<UniformBuffer>>& uniform_buffer)
    {
        m_bound_uniform_buffers.push_back(uniform_buffer); // TODO: map

        const std::vector<VkDescriptorBufferInfo> buffer_infos = uniform_buffer->get<UniformBuffer>().descriptor_info();

        assert(buffer_infos.size() == m_frame_counter.max_frame_index);

        for (std::size_t i = 0; i < m_frame_counter.max_frame_index; i++) {
            VkWriteDescriptorSet descriptor_write{};
            descriptor_write.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
            descriptor_write.dstSet = m_vk_handles[i];
            descriptor_write.dstBinding = binding;
            descriptor_write.dstArrayElement = 0;
            descriptor_write.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
            descriptor_write.descriptorCount = 1;
            descriptor_write.pBufferInfo = &buffer_infos[i];
            descriptor_write.pImageInfo = nullptr; // Optional
            descriptor_write.pTexelBufferView = nullptr; // Optional

            vkUpdateDescriptorSets(m_vk_device, 1, &descriptor_write, 0, nullptr);
        }
    }

    const VkDescriptorSet& DescriptorSet::vk_handle() const
    {
        return m_vk_handles[*m_frame_counter.curent_frame_index];
    }

    VkDescriptorSetLayout DescriptorSet::vk_layout() const
    {
        return m_layout->get<DescriptorSetLayout>().vk_handle();
    }
}
