#pragma once

#include <cstdint>
#include <optional>

#include <vulkan/vulkan.h>

#include "../../platform/desktop/GlfwWindow.h"
#include "../Handle.h"
#include "Drawable.h"
#include "FrameBuffer.h"
#include "FrameBufferCreator.h"
#include "FrameCounter.h"
#include "IndexBuffer.h"
#include "Pipeline.h"
#include "Texture2D.h"
#include "VertexBuffer.h"

namespace yage::gl::vulkan
{
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

        void flush_resources();

        FrameCounter swap_chain_counter() const;

        FrameCounter frames_in_flight_counter() const;

        [[nodiscard]] VkDevice device() const;

        [[nodiscard]] VkPhysicalDevice physical_device() const;

        [[nodiscard]] VkQueue graphics_queue() const;

        [[nodiscard]] FrameBuffer& swap_chain_frame_buffer() const;

        [[nodiscard]] std::shared_ptr<RenderPassHandle> swap_chain_render_pass() const;

        [[nodiscard]] VkCommandBuffer command_buffer_for_frame() const;

        const std::shared_ptr<Store<RenderPass, RenderPass>>& store_render_passes();

        const std::shared_ptr<Store<IFrameBuffer, FrameBuffer>>& store_frame_buffers();

        const std::shared_ptr<Store<Pipeline, Pipeline>>& store_pipelines();

        const std::shared_ptr<Store<IVertexBuffer, VertexBuffer>>& store_vertex_buffers();

        const std::shared_ptr<Store<IIndexBuffer, IndexBuffer>>& store_index_buffers();

        const std::shared_ptr<Store<IDrawable2, Drawable>>& store_drawables();

        const std::shared_ptr<Store<ITexture2D2, Texture2D>>& store_textures();

        [[nodiscard]] VkCommandPool command_pool() const;

        std::uint32_t find_memory_type(std::uint32_t typeFilter, VkMemoryPropertyFlags properties);

        void create_buffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties,
                           VkBuffer& buffer, VkDeviceMemory& buffer_memory);

        void copy_buffer(VkBuffer source, VkBuffer destination, VkDeviceSize size);

        void create_image(std::uint32_t width, std::uint32_t height, VkFormat format, VkImageTiling tiling,
                          VkImageUsageFlags usage, VkMemoryPropertyFlags properties, VkImage& image,
                          VkDeviceMemory& image_memory);

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

        VkInstance m_instance{};
        VkDebugUtilsMessengerEXT m_debug_messenger{};
        VkSurfaceKHR m_surface{};

        VkPhysicalDevice m_physical_device = VK_NULL_HANDLE;
        VkDevice m_device{};

        VkQueue m_graphics_queue{};
        VkQueue m_present_queue{};

        VkSwapchainKHR m_swap_chain{};
        VkFormat m_swap_chain_image_format{};
        VkExtent2D m_swap_chain_extent{};

        std::shared_ptr<RenderPassHandle> m_render_pass;

        unsigned int m_current_frame_in_flight = 0;
        unsigned int m_current_swap_chain_image_index = 0;

        std::vector<VkImage> m_swap_chain_images{};
        std::shared_ptr<ImageViewHandle> m_swap_chain_image_view{};
        FrameBufferHandle m_swap_chain_frame_buffer{};

        VkCommandPool m_command_pool{};
        std::vector<VkCommandBuffer> m_command_buffers{};

        std::vector<VkSemaphore> m_image_available_semaphores{};
        std::vector<VkSemaphore> m_render_finished_semaphores{};
        std::vector<VkFence> m_in_flight_fences{};

        bool m_framebuffer_resized = false;

        std::shared_ptr<Store<RenderPass, RenderPass>> m_store_render_passes =
                std::make_shared<Store<RenderPass, RenderPass>>();
        std::shared_ptr<Store<ImageView, ImageView>> m_store_image_views =
                std::make_shared<Store<ImageView, ImageView>>();
        std::shared_ptr<Store<IFrameBuffer, FrameBuffer>> m_store_frame_buffers =
                std::make_shared<Store<IFrameBuffer, FrameBuffer>>();
        std::shared_ptr<Store<IVertexBuffer, VertexBuffer>> m_store_vertex_buffers =
                std::make_shared<Store<IVertexBuffer, VertexBuffer>>();
        std::shared_ptr<Store<IIndexBuffer, IndexBuffer>> m_store_index_buffers =
                std::make_shared<Store<IIndexBuffer, IndexBuffer>>();
        std::shared_ptr<Store<IDrawable2, Drawable>> m_store_drawables =
                std::make_shared<Store<IDrawable2, Drawable>>();
        std::shared_ptr<Store<ITexture2D2, Texture2D>> m_store_textures =
                std::make_shared<Store<ITexture2D2, Texture2D>>();
        std::shared_ptr<Store<Pipeline, Pipeline>> m_store_pipelines = std::make_shared<Store<Pipeline, Pipeline>>();

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

        void create_swap_chain();

        void create_image_views();

        VkShaderModule createShaderModule(const std::vector<std::byte>& code);

        void create_swap_chain_render_pass();

        void create_framebuffers();

        void create_command_pool();

        void create_command_buffers();

        void create_sync_objects();

        void cleanupSwapChain();

        void recreateSwapChain();
    };
}
