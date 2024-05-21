#pragma once

#include <vulkan/vulkan.h>
#include "core/platform/desktop/GlfwWindow.h"

namespace gl::vulkan
{
    class Instance
    {
    public:
        explicit Instance();

        ~Instance();

    private:
        VkInstance m_instance{};
        VkDebugUtilsMessengerEXT m_debug_messenger{};

        static VKAPI_ATTR VkBool32 VKAPI_CALL debug_callback(
                VkDebugUtilsMessageSeverityFlagBitsEXT severity,
                VkDebugUtilsMessageTypeFlagsEXT type,
                const VkDebugUtilsMessengerCallbackDataEXT* callback_data,
                void* user_data);

        static bool checkValidationLayerSupport(const std::vector<const char*>& validationLayers);

        static std::vector<const char*> required_extensions();

        void setup_debug_messenger();

        VkDebugUtilsMessengerCreateInfoEXT populate_debug_messenger_create_info();
    };
}
