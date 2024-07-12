#pragma once

#include <cstdint>
#include <optional>

#include <vulkan/vulkan.h>

#include "../../platform/desktop/GlfwWindow.h"
#include "../Handle.h"
#include "DescriptorAllocator.h"
#include "DescriptorSetLayout.h"
#include "Drawable.h"
#include "FrameBuffer.h"
#include "FrameBufferCreator.h"
#include "FrameCounter.h"
#include "IndexBuffer.h"
#include "Pipeline.h"
#include "SwapChain.h"
#include "Texture2D.h"
#include "UniformBuffer.h"
#include "VertexBuffer.h"

namespace yage::gl::vulkan
{
    using UniformBufferStore = Store<IUniformBuffer, UniformBuffer>;
    using VertexBufferStore = Store<IVertexBuffer, VertexBuffer>;
    using IndexBufferStore = Store<IIndexBuffer, IndexBuffer>;
    using FrameBufferStore = Store<IFrameBuffer, FrameBuffer>;

    using ImageViewStore = Store<ImageView, ImageView>;
    using Texture2DStore = Store<ITexture2D2, Texture2D>;

    using DrawableStore = Store<IDrawable2, Drawable>;

    using DescriptorSetLayoutStore = Store<IDescriptorSetLayout, DescriptorSetLayout>;
    using RenderPassStore = Store<RenderPass, RenderPass>;
    using PipelineStore = Store<Pipeline, Pipeline>;

    class DescriptorSet;

    struct QueueFamilyIndices
    {
        std::optional<uint32_t> graphicsFamily;
        std::optional<uint32_t> presentFamily;

        [[nodiscard]] bool is_complete() const
        {
            return graphicsFamily.has_value() && presentFamily.has_value();
        }
    };

    struct SwapChainSupportDetails
    {
        VkSurfaceCapabilitiesKHR capabilities;
        std::vector<VkSurfaceFormatKHR> formats;
        std::vector<VkPresentModeKHR> presentModes;
    };

    class Instance final : public std::enable_shared_from_this<Instance>
    {
    public:
        explicit Instance(std::weak_ptr<platform::desktop::GlfwWindow> window);

        ~Instance();

        void initialize();

        void prepare_frame();

        void present_frame();

        void flush_gpu();

        FrameCounter frames_in_flight_counter() const;

        FrameCounter static_counter() const;

        [[nodiscard]] VkDevice device() const;

        [[nodiscard]] VkPhysicalDevice physical_device() const;

        [[nodiscard]] VkQueue graphics_queue() const;

        [[nodiscard]] VkCommandBuffer command_buffer_for_frame() const;

        const SwapChain& swap_chain() const;

        RenderPassStore& store_render_passes();

        FrameBufferStore& store_frame_buffers();

        PipelineStore& store_pipelines();

        VertexBufferStore& store_vertex_buffers();

        IndexBufferStore& store_index_buffers();

        UniformBufferStore& store_uniform_buffers();

        DrawableStore& store_drawables();

        Texture2DStore& store_textures();

        DescriptorSetLayoutStore& store_descriptor_set_layouts();

        DescriptorAllocator& descriptor_allocator();

        [[nodiscard]] VkCommandPool command_pool() const;

        std::uint32_t find_memory_type(std::uint32_t typeFilter, VkMemoryPropertyFlags properties);

        void create_buffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties,
                           VkBuffer& buffer, VkDeviceMemory& buffer_memory);

        void copy_buffer(VkBuffer source, VkBuffer destination, VkDeviceSize size);

        void create_image(std::uint32_t width, std::uint32_t height, VkFormat format, VkImageTiling tiling,
                          VkImageUsageFlags usage, VkMemoryPropertyFlags properties, VkImage& image,
                          VkDeviceMemory& image_memory);

        VkImageView create_image_view(VkImage image, VkFormat format, VkImageAspectFlags aspect_flags);

        VkSampler create_texture_sampler(const TextureSampler& sampler);

        VkCommandBuffer begin_one_time_command_buffer();

        void end_one_time_command_buffer(VkCommandBuffer command_buffer);

        void transition_image_layout(VkImage image, VkFormat format, VkImageLayout old_layout,
                                     VkImageLayout new_layout);

        void copy_buffer_to_image(VkBuffer buffer, VkImage image, std::uint32_t width, std::uint32_t height);

    private:
        const unsigned int MAX_FRAMES_IN_FLIGHT = 2;
        const std::vector<const char*> VALIDATION_LAYERS = {"VK_LAYER_KHRONOS_validation"};
        const std::vector<const char*> DEVICE_EXTENSIONS = {VK_KHR_SWAPCHAIN_EXTENSION_NAME};

        std::weak_ptr<platform::desktop::GlfwWindow> m_window;

        VkInstance m_instance = VK_NULL_HANDLE;
        VkDebugUtilsMessengerEXT m_debug_messenger = VK_NULL_HANDLE;
        VkSurfaceKHR m_surface = VK_NULL_HANDLE;

        VkPhysicalDevice m_physical_device = VK_NULL_HANDLE;
        VkDevice m_device = VK_NULL_HANDLE;

        VkQueue m_graphics_queue = VK_NULL_HANDLE;
        VkQueue m_present_queue = VK_NULL_HANDLE;

        SwapChain m_swap_chain;

        const unsigned int m_current_frame_static = 0;
        unsigned int m_current_frame_in_flight = 0;

        VkCommandPool m_command_pool = VK_NULL_HANDLE;
        std::vector<VkCommandBuffer> m_command_buffers;

        std::vector<VkSemaphore> m_image_available_semaphores;
        std::vector<VkSemaphore> m_render_finished_semaphores;
        std::vector<VkFence> m_in_flight_fences;

        bool m_framebuffer_resized = false;

        std::optional<DescriptorAllocator> m_descriptor_allocator;

        std::shared_ptr<FrameBufferStore> m_store_frame_buffers = std::make_shared<FrameBufferStore>();
        std::shared_ptr<VertexBufferStore> m_store_vertex_buffers = std::make_shared<VertexBufferStore>();
        std::shared_ptr<IndexBufferStore> m_store_index_buffers = std::make_shared<IndexBufferStore>();
        std::shared_ptr<UniformBufferStore> m_store_uniform_buffers = std::make_shared<UniformBufferStore>();

        std::shared_ptr<ImageViewStore> m_store_image_views = std::make_shared<ImageViewStore>();
        std::shared_ptr<Texture2DStore> m_store_textures = std::make_shared<Texture2DStore>();

        std::shared_ptr<DrawableStore> m_store_drawables = std::make_shared<DrawableStore>();

        std::shared_ptr<DescriptorSetLayoutStore> m_store_descriptor_set_layouts =
                std::make_shared<DescriptorSetLayoutStore>();
        std::shared_ptr<RenderPassStore> m_store_render_passes = std::make_shared<RenderPassStore>();
        std::shared_ptr<PipelineStore> m_store_pipelines = std::make_shared<PipelineStore>();

        void create_instance();

        static VKAPI_ATTR VkBool32 VKAPI_CALL debug_callback(VkDebugUtilsMessageSeverityFlagBitsEXT severity,
                                                             VkDebugUtilsMessageTypeFlagsEXT type,
                                                             const VkDebugUtilsMessengerCallbackDataEXT* callback_data,
                                                             void* user_data);

        static bool checkValidationLayerSupport(const std::vector<const char*>& validationLayers);

        static std::vector<const char*> required_extensions();

        void setup_debug_messenger();

        VkDebugUtilsMessengerCreateInfoEXT populate_debug_messenger_create_info();

        void create_surface();

        void pick_physical_device();

        bool isDeviceSuitable(VkPhysicalDevice device);

        bool checkDeviceExtensionSupport(VkPhysicalDevice device);

        QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device);

        void create_logical_device();

        SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice device);

        static VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);

        static VkPresentModeKHR chooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes);

        VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities);

        VkFormat findSupportedFormat(const std::vector<VkFormat>& candidates, VkImageTiling tiling,
                                     VkFormatFeatureFlags features);

        VkFormat findDepthFormat();

        void create_swap_chain();

        void create_command_pool();

        void create_command_buffers();

        void create_sync_objects();

        void recreateSwapChain();

        void create_descriptor_allocator();
    };
}
