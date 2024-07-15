#include "Texture2D.h"
#include "Instance.h"
#include "enums.h"

namespace yage::gl::vulkan
{
    Texture2D::Texture2D(Instance* instance, const FrameCounter frame_counter, const TextureSampler& sampler,
                         const PixelTransferInfo& data_info, const std::span<const std::byte> data)
        : m_instance(instance),
          m_vk_device(m_instance->device()),
          m_frame_counter(frame_counter)
    {
        const VkDeviceSize buffer_size = data.size();

        m_width = data_info.width;
        m_height = data_info.height;

        if (sampler.mip_map_mode != MipMapMode::NONE) {
            m_mip_levels =
                    static_cast<unsigned int>(std::floor(std::log2(std::max(data_info.width, data_info.height)))) + 1;
        } else {
            m_mip_levels = 1; // even without mip mapping we need one level for the original image
        }

        VkBuffer staging_buffer;
        VkDeviceMemory staging_buffer_memory;
        m_instance->create_buffer(buffer_size, VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
                                  VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
                                  staging_buffer, staging_buffer_memory);

        m_vk_images.resize(m_frame_counter.max_frame_index);
        m_vk_image_views.resize(m_frame_counter.max_frame_index);
        m_vk_memories.resize(m_frame_counter.max_frame_index);

        for (unsigned int i = 0; i < m_frame_counter.max_frame_index; ++i) {
            void* mapped_data;
            vkMapMemory(m_vk_device, staging_buffer_memory, 0, buffer_size, 0, &mapped_data);
            memcpy(mapped_data, data.data(), buffer_size);
            vkUnmapMemory(m_vk_device, staging_buffer_memory);

            const VkFormat vk_image_format = convert(data_info.image_format);

            m_instance->create_image(
                    m_width, m_height, m_mip_levels, VK_SAMPLE_COUNT_1_BIT, vk_image_format, VK_IMAGE_TILING_OPTIMAL,
                    VK_IMAGE_USAGE_TRANSFER_SRC_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT,
                    VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, m_vk_images[i], m_vk_memories[i]);

            m_instance->transition_image_layout(m_vk_images[i], vk_image_format, VK_IMAGE_LAYOUT_UNDEFINED,
                                                VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, m_mip_levels);

            m_instance->copy_buffer_to_image(staging_buffer, m_vk_images[i], m_width, m_height);

            // transitioned to VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL while generating mipmaps
            m_instance->generate_mip_maps(m_vk_images[i], vk_image_format, m_width, m_height, m_mip_levels);

            m_vk_image_views[i] = instance->create_image_view(m_vk_images[i], vk_image_format,
                                                              VK_IMAGE_ASPECT_COLOR_BIT, m_mip_levels);
        }

        m_vk_sampler = instance->create_texture_sampler(sampler, m_mip_levels);

        vkDestroyBuffer(m_vk_device, staging_buffer, nullptr);
        vkFreeMemory(m_vk_device, staging_buffer_memory, nullptr);
    }

    Texture2D::Texture2D(Instance* instance, const FrameCounter frame_counter, const TextureSampler& sampler,
                         const PixelTransferInfo& data_info, const VkImageUsageFlags usage,
                         const VkImageLayout layout, const VkSampleCountFlagBits msaa_samples)
        : m_instance(instance),
          m_vk_device(m_instance->device()),
          m_frame_counter(frame_counter)
    {
        m_width = data_info.width;
        m_height = data_info.height;
        m_mip_levels = 1; // no mip-mapping

        VkImageAspectFlags aspect = VK_IMAGE_ASPECT_NONE;
        if (usage & VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT || usage & VK_IMAGE_USAGE_SAMPLED_BIT) {
            aspect |= VK_IMAGE_ASPECT_COLOR_BIT;
        }
        if (usage & VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT) {
            aspect |= VK_IMAGE_ASPECT_DEPTH_BIT | VK_IMAGE_ASPECT_STENCIL_BIT;
        }

        for (unsigned int i = 0; i < m_frame_counter.max_frame_index; ++i) {
            const VkFormat vk_image_format = convert(data_info.image_format);

            m_instance->create_image(m_width, m_height, m_mip_levels, msaa_samples, vk_image_format,
                                     VK_IMAGE_TILING_OPTIMAL, usage, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
                                     m_vk_images[i], m_vk_memories[i]);

            m_instance->transition_image_layout(m_vk_images[i], vk_image_format, VK_IMAGE_LAYOUT_UNDEFINED, layout,
                                                m_mip_levels);

            m_vk_image_views[i] = instance->create_image_view(m_vk_images[i], vk_image_format, aspect, m_mip_levels);
        }

        m_vk_sampler = instance->create_texture_sampler(sampler, m_mip_levels);
    }

    Texture2D::~Texture2D()
    {
        for (unsigned int i = 0; i < m_frame_counter.max_frame_index; ++i) {
            vkDestroySampler(m_vk_device, m_vk_sampler, nullptr);
            vkDestroyImageView(m_vk_device, m_vk_image_views[i], nullptr);
            vkDestroyImage(m_vk_device, m_vk_images[i], nullptr);
            vkFreeMemory(m_vk_device, m_vk_memories[i], nullptr);
        }
    }

    Texture2D::Texture2D(Texture2D&& other) noexcept
        : m_instance(other.m_instance),
          m_vk_device(other.m_vk_device),
          m_frame_counter(other.m_frame_counter),
          m_vk_memories(std::move(other.m_vk_memories)),
          m_vk_images(std::move(other.m_vk_images)),
          m_vk_image_views(std::move(other.m_vk_image_views)),
          m_vk_sampler(other.m_vk_sampler)
    {
        other.m_vk_device = VK_NULL_HANDLE;
        other.m_vk_images.clear();
        other.m_vk_memories.clear();
        other.m_vk_image_views.clear();
        other.m_vk_sampler = VK_NULL_HANDLE;
    }

    Texture2D& Texture2D::operator=(Texture2D&& other) noexcept
    {
        if (this == &other)
            return *this;

        m_instance = other.m_instance;
        m_vk_device = other.m_vk_device;
        m_frame_counter = other.m_frame_counter;
        m_vk_memories = std::move(other.m_vk_memories);
        m_vk_images = std::move(other.m_vk_images);
        m_vk_image_views = std::move(other.m_vk_image_views);
        m_vk_sampler = other.m_vk_sampler;

        other.m_vk_device = VK_NULL_HANDLE;
        other.m_vk_images.clear();
        other.m_vk_memories.clear();
        other.m_vk_image_views.clear();
        other.m_vk_sampler = VK_NULL_HANDLE;

        return *this;
    }

    VkSampler Texture2D::vk_sampler() const
    {
        return m_vk_sampler;
    }

    std::vector<VkDescriptorImageInfo> Texture2D::descriptor_info() const
    {
        std::vector<VkDescriptorImageInfo> image_infos;
        image_infos.reserve(m_frame_counter.max_frame_index);
        for (unsigned int i = 0; i < m_frame_counter.max_frame_index; ++i) {
            VkDescriptorImageInfo info{};
            info.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
            info.imageView = m_vk_image_views[i];
            info.sampler = m_vk_sampler;

            image_infos.push_back(info);
        }
        return image_infos;
    }

    VkImageView Texture2D::vk_image_view(const unsigned int instance) const
    {
        return m_vk_image_views[instance];
    }
}
