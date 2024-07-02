#pragma once

#include <optional>
#include <cstdint>

#include <vulkan/vulkan.h>

#include "../../platform/desktop/GlfwWindow.h"
#include "FrameBuffer.h"
#include "FrameBufferFactory.h"
#include "Pipeline.h"
#include "PipelineBuilder.h"

namespace yage::gl::vulkan
{
    struct QueueFamilyIndices
    {
        std::optional<uint32_t> graphicsFamily;
        std::optional<uint32_t> presentFamily;

        [[nodiscard]]
        bool is_complete() const
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

        [[nodiscard]] VkDevice device() const { return m_device; }

        [[nodiscard]] VkPhysicalDevice physical_device() const { return m_physical_device; }

        [[nodiscard]] FrameBuffer& swap_chain_frame_buffer_for_frame() const { return *m_swap_chain_frame_buffers[m_current_swap_chain_image_index]; }

        [[nodiscard]] std::shared_ptr<RenderPass> swap_chain_render_pass() const { return m_render_pass; }

        [[nodiscard]] VkCommandBuffer command_buffer_for_frame() const { return m_command_buffers[m_current_frame]; }

    private:
        const std::vector<const char*> m_validation_layers = {
                "VK_LAYER_KHRONOS_validation"
        };
        const std::vector<const char*> m_device_extensions = {
                VK_KHR_SWAPCHAIN_EXTENSION_NAME
        };
        const int MAX_FRAMES_IN_FLIGHT = 2;

        std::weak_ptr<platform::desktop::GlfwWindow> m_window;
        VkInstance m_instance{};
        VkDebugUtilsMessengerEXT m_debug_messenger{};
        VkSurfaceKHR m_surface{};
        VkPhysicalDevice m_physical_device = VK_NULL_HANDLE;
        VkDevice m_device{};
        VkQueue graphicsQueue{};
        VkQueue presentQueue{};
        VkSwapchainKHR swapChain{};

        VkFormat swapChainImageFormat{};
        VkExtent2D swapChainExtent{};

        std::shared_ptr<RenderPass> m_render_pass;

        std::uint32_t m_current_frame = 0;
        std::uint32_t m_current_swap_chain_image_index = 0;

        std::vector<VkImage> m_swap_chain_images{};
        std::vector<std::unique_ptr<ImageView>> m_swap_chain_image_views{};
        std::vector<std::unique_ptr<FrameBuffer>> m_swap_chain_frame_buffers{};

        VkCommandPool m_command_pool{};
        std::vector<VkCommandBuffer> m_command_buffers{};

        std::vector<VkSemaphore> imageAvailableSemaphores{};
        std::vector<VkSemaphore> renderFinishedSemaphores{};
        std::vector<VkFence> inFlightFences{};

        bool framebufferResized = false;

        void create_instance();

        static VKAPI_ATTR VkBool32 VKAPI_CALL debug_callback(
                VkDebugUtilsMessageSeverityFlagBitsEXT severity,
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

        void create_render_pass();

        void create_framebuffers();

        void create_command_pool();

        void create_command_buffers();

        void create_sync_objects();

        void cleanupSwapChain();

        void recreateSwapChain();
    };
}
