#pragma once

#include <cstdint>
#include <optional>

#include <vulkan/vulkan.h>

#include "../../platform/desktop/GlfwWindow.h"
#include "../Handle.h"
#include "FrameBuffer.h"
#include "FrameBufferFactory.h"
#include "Pipeline.h"
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

        [[nodiscard]] VkDevice device() const
        {
            return m_device;
        }

        [[nodiscard]] VkPhysicalDevice physical_device() const
        {
            return m_physical_device;
        }

        [[nodiscard]] VkQueue graphics_queue() const
        {
            return m_graphics_queue;
        }

        [[nodiscard]] FrameBuffer& swap_chain_frame_buffer_for_frame() const
        {
            return m_swap_chain_frame_buffers[m_current_swap_chain_image_index].get<FrameBuffer>();
        }

        [[nodiscard]] std::shared_ptr<RenderPassHandle> swap_chain_render_pass() const
        {
            return m_render_pass;
        }

        [[nodiscard]] VkCommandBuffer command_buffer_for_frame() const
        {
            return m_command_buffers[m_current_frame];
        }

        const std::shared_ptr<Store<RenderPass, RenderPass>>& store_render_passes()
        {
            return m_store_render_passes;
        }

        const std::shared_ptr<Store<IFrameBuffer, FrameBuffer>>& store_frame_buffers()
        {
            return m_store_frame_buffers;
        }

        const std::shared_ptr<Store<Pipeline, Pipeline>>& store_pipelines()
        {
            return m_store_pipelines;
        }

        const std::shared_ptr<Store<IVertexBuffer, VertexBuffer>>& store_vertex_buffers()
        {
            return m_store_vertex_buffers;
        }

    private:
        const std::vector<const char*> VALIDATION_LAYERS = {"VK_LAYER_KHRONOS_validation"};
        const std::vector<const char*> DEVICE_EXTENSIONS = {VK_KHR_SWAPCHAIN_EXTENSION_NAME};
        const int MAX_FRAMES_IN_FLIGHT = 2;

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

        std::uint32_t m_current_frame = 0;
        std::uint32_t m_current_swap_chain_image_index = 0;

        std::vector<VkImage> m_swap_chain_images{};
        std::vector<ImageViewHandle> m_swap_chain_image_views{};
        std::vector<FrameBufferHandle> m_swap_chain_frame_buffers{};

        VkCommandPool m_command_pool{};
        std::vector<VkCommandBuffer> m_command_buffers{};

        std::vector<VkSemaphore> m_image_available_semaphores{};
        std::vector<VkSemaphore> m_render_finished_semaphores{};
        std::vector<VkFence> m_in_flight_fences{};

        bool m_framebuffer_resized = false;

        std::shared_ptr<Store<RenderPass, RenderPass>> m_store_render_passes = std::make_shared<Store<RenderPass, RenderPass>>();
        std::shared_ptr<Store<ImageView, ImageView>> m_store_image_views = std::make_shared<Store<ImageView, ImageView>>();
        std::shared_ptr<Store<IFrameBuffer, FrameBuffer>> m_store_frame_buffers = std::make_shared<Store<IFrameBuffer, FrameBuffer>>();
        std::shared_ptr<Store<IVertexBuffer, VertexBuffer>> m_store_vertex_buffers = std::make_shared<Store<IVertexBuffer, VertexBuffer>>();
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
