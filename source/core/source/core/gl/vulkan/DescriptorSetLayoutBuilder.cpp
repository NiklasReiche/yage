#include "DescriptorSetLayoutBuilder.h"
#include "Instance.h"

namespace yage::gl::vulkan
{
    DescriptorSetLayoutBuilder::DescriptorSetLayoutBuilder(std::weak_ptr<Instance> instance)
        : m_instance(std::move(instance))
    {
    }

    DescriptorSetLayoutBuilder& DescriptorSetLayoutBuilder::with_uniform_buffer()
    {
        return with_uniform_buffer_array(1);
    }

    DescriptorSetLayoutBuilder& DescriptorSetLayoutBuilder::with_uniform_buffer_array(const std::uint32_t count)
    {
        VkDescriptorSetLayoutBinding ubo_layout_binding{};
        ubo_layout_binding.binding = m_binding_number;
        ubo_layout_binding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
        ubo_layout_binding.descriptorCount = count;
        ubo_layout_binding.stageFlags = VK_SHADER_STAGE_VERTEX_BIT; // TODO
        ubo_layout_binding.pImmutableSamplers = nullptr; // Optional

        m_bindings.push_back(ubo_layout_binding);
        m_binding_number++;

        return *this;
    }

    Handle<DescriptorSetLayout> DescriptorSetLayoutBuilder::build()
    {
        m_create_info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
        m_create_info.bindingCount = m_bindings.size();
        m_create_info.pBindings = m_bindings.data();

        const auto instance = m_instance.lock();
        return instance->store_descriptor_set_layouts()->create(instance.get(), m_create_info);
    }

    void DescriptorSetLayoutBuilder::clear()
    {
        m_create_info = VkDescriptorSetLayoutCreateInfo{};
        m_bindings.clear();
        m_binding_number = 0;
    }
}
