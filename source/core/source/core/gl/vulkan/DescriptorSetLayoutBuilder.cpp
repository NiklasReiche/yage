#include "DescriptorSetLayoutBuilder.h"
#include "Instance.h"

namespace yage::gl::vulkan
{
    DescriptorSetLayoutBuilder::DescriptorSetLayoutBuilder(std::weak_ptr<Instance> instance)
        : m_instance(std::move(instance))
    {
    }

    IDescriptorSetLayoutBuilder& DescriptorSetLayoutBuilder::with_uniform_buffer_at(const unsigned int binding)
    {
        return with_uniform_buffer_array_at(binding, 1);
    }

    DescriptorSetLayoutBuilder& DescriptorSetLayoutBuilder::with_uniform_buffer_array_at(const unsigned int binding,
                                                                                         const std::uint32_t count)
    {
        VkDescriptorSetLayoutBinding ubo_layout_binding{};
        ubo_layout_binding.binding = static_cast<std::uint32_t>(binding);
        ubo_layout_binding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
        ubo_layout_binding.descriptorCount = count;
        ubo_layout_binding.stageFlags = VK_SHADER_STAGE_VERTEX_BIT; // TODO
        ubo_layout_binding.pImmutableSamplers = nullptr; // Optional

        m_bindings.push_back(ubo_layout_binding);

        return *this;
    }

    IDescriptorSetLayoutBuilder& DescriptorSetLayoutBuilder::with_texture_sampler_at(const unsigned int binding)
    {
        VkDescriptorSetLayoutBinding sampler_layout_binding{};
        sampler_layout_binding.binding = static_cast<std::uint32_t>(binding);
        sampler_layout_binding.descriptorCount = 1;
        sampler_layout_binding.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
        sampler_layout_binding.pImmutableSamplers = nullptr;
        sampler_layout_binding.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT; // TODO

        m_bindings.push_back(sampler_layout_binding);

        return *this;
    }

    DescriptorSetLayoutHandle DescriptorSetLayoutBuilder::build()
    {
        m_create_info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
        m_create_info.bindingCount = m_bindings.size();
        m_create_info.pBindings = m_bindings.data();

        const auto instance = m_instance.lock();
        return instance->store_descriptor_set_layouts().create(instance.get(), m_create_info);
    }

    void DescriptorSetLayoutBuilder::clear()
    {
        m_create_info = VkDescriptorSetLayoutCreateInfo{};
        m_bindings.clear();
    }
}
