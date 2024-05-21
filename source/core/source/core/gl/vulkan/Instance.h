#pragma once

#include <optional>

#include <vulkan/vulkan.h>

#include "../../platform/desktop/GlfwWindow.h"

namespace gl::vulkan
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

    class Instance
    {
    public:
        explicit Instance(std::weak_ptr<platform::desktop::GlfwWindow> window);

        ~Instance();

    private:
        const std::vector<const char*> m_validation_layers = {
                "VK_LAYER_KHRONOS_validation"
        };
        const std::vector<const char*> m_device_extensions = {
                VK_KHR_SWAPCHAIN_EXTENSION_NAME
        };

        std::weak_ptr<platform::desktop::GlfwWindow> m_window;
        VkInstance m_instance{};
        VkDebugUtilsMessengerEXT m_debug_messenger{};
        VkSurfaceKHR m_surface{};
        VkPhysicalDevice m_physical_device = VK_NULL_HANDLE;
        VkDevice m_device{};
        VkQueue graphicsQueue{};
        VkQueue presentQueue{};
        VkSwapchainKHR swapChain{};
        std::vector<VkImage> swapChainImages{};
        VkFormat swapChainImageFormat{};
        VkExtent2D swapChainExtent{};
        std::vector<VkImageView> swapChainImageViews{};

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

        VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);

        VkPresentModeKHR chooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes);

        VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities);

        void create_swap_chain();

        void create_image_views();
    };
}
