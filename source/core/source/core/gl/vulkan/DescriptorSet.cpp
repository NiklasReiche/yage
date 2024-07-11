#include "DescriptorSet.h"
#include "Instance.h"

namespace yage::gl::vulkan
{
    DescriptorSet::DescriptorSet(Instance* instance, const VkDescriptorSetAllocateInfo& alloc_info,
                                 const FrameCounter frame_counter, DescriptorSetLayoutHandle layout)
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

    void DescriptorSet::write(const unsigned int binding, const UniformBufferHandle& uniform_buffer)
    {
        m_bound_uniform_buffers.emplace(static_cast<std::uint32_t>(binding), uniform_buffer);

        const std::vector<VkDescriptorBufferInfo> buffer_infos = uniform_buffer.get<UniformBuffer>().descriptor_info();

        assert(buffer_infos.size() == 1 || buffer_infos.size() == m_frame_counter.max_frame_index);

        std::vector<VkWriteDescriptorSet> descriptor_writes;
        descriptor_writes.reserve(m_frame_counter.max_frame_index);
        for (unsigned int i = 0; i < m_frame_counter.max_frame_index; i++) {
            VkWriteDescriptorSet descriptor_write{};
            descriptor_write.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
            descriptor_write.dstSet = m_vk_handles[i];
            descriptor_write.dstBinding = static_cast<std::uint32_t>(binding);
            descriptor_write.dstArrayElement = 0;
            descriptor_write.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
            descriptor_write.descriptorCount = 1;
            // if the uniform buffer is static, then use the same reference for each instance
            descriptor_write.pBufferInfo = &buffer_infos[buffer_infos.size() == 1 ? 1 : i];

            descriptor_writes.push_back(descriptor_write);
        }
        vkUpdateDescriptorSets(m_vk_device, descriptor_writes.size(), &descriptor_writes[0], 0, nullptr);
    }

    void DescriptorSet::write(const unsigned int binding, const Texture2DHandle& texture_2d)
    {
        m_bound_textures_2d.emplace(static_cast<std::uint32_t>(binding), texture_2d);

        const std::vector<VkDescriptorImageInfo> image_infos = texture_2d.get<Texture2D>().descriptor_info();

        assert(image_infos.size() == 1 || image_infos.size() == m_frame_counter.max_frame_index);

        std::vector<VkWriteDescriptorSet> descriptor_writes;
        descriptor_writes.reserve(m_frame_counter.max_frame_index);
        for (unsigned int i = 0; i < m_frame_counter.max_frame_index; i++) {
            VkWriteDescriptorSet descriptor_write{};
            descriptor_write.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
            descriptor_write.dstSet = m_vk_handles[i];
            descriptor_write.dstBinding = static_cast<std::uint32_t>(binding);
            descriptor_write.dstArrayElement = 0;
            descriptor_write.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
            descriptor_write.descriptorCount = 1;
            // if the texture is static, then use the same reference for each instance
            descriptor_write.pImageInfo = &image_infos[image_infos.size() == 1 ? 1 : i];

            descriptor_writes.push_back(descriptor_write);
        }
        vkUpdateDescriptorSets(m_vk_device, descriptor_writes.size(), &descriptor_writes[0], 0, nullptr);
    }

    const VkDescriptorSet& DescriptorSet::vk_handle() const
    {
        return m_vk_handles[*m_frame_counter.curent_frame_index];
    }

    VkDescriptorSetLayout DescriptorSet::vk_layout() const
    {
        return m_layout.get<DescriptorSetLayout>().vk_handle();
    }
}
