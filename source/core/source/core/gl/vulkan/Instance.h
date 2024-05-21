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

    class Instance
    {
    public:
        explicit Instance(std::weak_ptr<platform::desktop::GlfwWindow> window);

        ~Instance();

    private:
        const std::vector<const char*> m_validation_layers = {
                "VK_LAYER_KHRONOS_validation"
        };

        std::weak_ptr<platform::desktop::GlfwWindow> m_window;
        VkInstance m_instance{};
        VkDebugUtilsMessengerEXT m_debug_messenger{};
        VkSurfaceKHR m_surface{};
        VkPhysicalDevice m_physical_device = VK_NULL_HANDLE;
        VkDevice m_device{};
        VkQueue graphicsQueue{};
        VkQueue presentQueue{};

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

        QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device);

        void create_logical_device();
    };
}
