#include "Instance.h"
#include <GLFW/glfw3.h>
#include <cstdint>
#include <cstring>
#include <limits>
#include <set>
#include <utility>

namespace yage::gl::vulkan
{
    VkResult CreateDebugUtilsMessengerEXT(const VkInstance instance,
                                          const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo,
                                          const VkAllocationCallbacks* pAllocator,
                                          VkDebugUtilsMessengerEXT* pDebugMessenger)
    {
        const auto func = reinterpret_cast<PFN_vkCreateDebugUtilsMessengerEXT>(
                vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT"));
        if (func != nullptr) {
            return func(instance, pCreateInfo, pAllocator, pDebugMessenger);
        }

        return VK_ERROR_EXTENSION_NOT_PRESENT;
    }

    void DestroyDebugUtilsMessengerEXT(const VkInstance instance, const VkDebugUtilsMessengerEXT debugMessenger,
                                       const VkAllocationCallbacks* pAllocator)
    {
        const auto func = reinterpret_cast<PFN_vkDestroyDebugUtilsMessengerEXT>(
                vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT"));
        if (func != nullptr) {
            func(instance, debugMessenger, pAllocator);
        }
    }

    Instance::Instance(std::weak_ptr<platform::desktop::GlfwWindow> window)
        : m_window(std::move(window))
    {
        m_window.lock()->attach_on_framebuffer_resize([this](int, int) { this->m_framebuffer_resized = true; });
    }

    Instance::~Instance()
    {
        m_store_frame_buffers->clear();
        m_store_image_views->clear();
        m_store_pipelines->clear();
        m_store_render_passes->clear();
        m_store_vertex_buffers->clear();
        m_store_index_buffers->clear();
        m_store_uniform_buffers->clear();
        m_store_drawables->clear();

        m_descriptor_allocator.value().reset();
        m_store_descriptor_set_layouts->clear();

        for (std::size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
            vkDestroySemaphore(m_device, m_render_finished_semaphores[i], nullptr);
            vkDestroySemaphore(m_device, m_image_available_semaphores[i], nullptr);
            vkDestroyFence(m_device, m_in_flight_fences[i], nullptr);
        }

        vkDestroyCommandPool(m_device, m_command_pool, nullptr);

        vkDestroyDevice(m_device, nullptr);

        vkDestroySurfaceKHR(m_instance, m_surface, nullptr);

#ifndef NDEBUG
        DestroyDebugUtilsMessengerEXT(m_instance, m_debug_messenger, nullptr);
#endif

        vkDestroyInstance(m_instance, nullptr);
    }

    void Instance::initialize()
    {
        create_instance();
        setup_debug_messenger();
        create_surface();
        pick_physical_device();
        create_logical_device();
        create_swap_chain();
        create_image_views();
        create_swap_chain_render_pass();
        create_framebuffers();
        create_command_pool();
        create_command_buffers();
        create_sync_objects();
        create_descriptor_allocator();
    }

    void Instance::create_instance()
    {
        VkApplicationInfo appInfo{}; // TODO
        appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
        appInfo.pApplicationName = "Hello Triangle";
        appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
        appInfo.pEngineName = "No Engine";
        appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
        appInfo.apiVersion = VK_API_VERSION_1_0;

        VkInstanceCreateInfo createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
        createInfo.pApplicationInfo = &appInfo;

        // load extensions
        const std::vector<const char*> extensions = required_extensions();
        createInfo.enabledExtensionCount = static_cast<std::uint32_t>(extensions.size());
        createInfo.ppEnabledExtensionNames = extensions.data();

#ifdef NDEBUG
        createInfo.enabledLayerCount = 0;
        createInfo.pNext = nullptr;
#else
        if (!checkValidationLayerSupport(VALIDATION_LAYERS)) {
            throw std::runtime_error("validation layers requested, but not available!");
        }

        createInfo.enabledLayerCount = static_cast<uint32_t>(VALIDATION_LAYERS.size());
        createInfo.ppEnabledLayerNames = VALIDATION_LAYERS.data();

        const VkDebugUtilsMessengerCreateInfoEXT debug_create_info = populate_debug_messenger_create_info();
        createInfo.pNext = &debug_create_info;
#endif

        if (vkCreateInstance(&createInfo, nullptr, &m_instance) != VK_SUCCESS) {
            throw std::runtime_error("failed to create vulkan instance");
        }
    }

    bool Instance::checkValidationLayerSupport(const std::vector<const char*>& validationLayers)
    {
        std::uint32_t layerCount;
        vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

        std::vector<VkLayerProperties> availableLayers(layerCount);
        vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

        for (const char* layerName: validationLayers) {
            bool layerFound = false;

            for (const auto& layerProperties: availableLayers) {
                if (std::strcmp(layerName, layerProperties.layerName) == 0) {
                    layerFound = true;
                    break;
                }
            }

            if (!layerFound) {
                return false;
            }
        }

        return true;
    }

    std::vector<const char*> Instance::required_extensions()
    {
        std::uint32_t glfwExtensionCount = 0;
        const char** glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);
        std::vector extensions(glfwExtensions, glfwExtensions + glfwExtensionCount);

#ifndef NDEBUG
        extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
#endif

        return extensions;
    }

    VkBool32 VKAPI_CALL Instance::debug_callback(const VkDebugUtilsMessageSeverityFlagBitsEXT severity,
                                                 VkDebugUtilsMessageTypeFlagsEXT,
                                                 const VkDebugUtilsMessengerCallbackDataEXT* callback_data, void*)
    {
        std::ostream& ostream = severity >= VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT ? std::cerr : std::cout;

        ostream << "Vulkan validation layer ";
        switch (severity) {
            case VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT:   ostream << "[ERROR]: "; break;
            case VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT: ostream << "[WARNING]: "; break;
            case VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT:    ostream << "[INFO]: "; break;
            case VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT: ostream << "[VERBOSE]: "; break;
            default:                                              break;
        }
        ostream << callback_data->pMessage << std::endl;

        return VK_FALSE;
    }

    VkDebugUtilsMessengerCreateInfoEXT Instance::populate_debug_messenger_create_info()
    {
        VkDebugUtilsMessengerCreateInfoEXT createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
        createInfo.messageSeverity =
                VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
        createInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT |
                                 VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT |
                                 VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
        createInfo.pfnUserCallback = debug_callback;
        createInfo.pUserData = this;
        return createInfo;
    }

    void Instance::setup_debug_messenger()
    {
        const VkDebugUtilsMessengerCreateInfoEXT createInfo = populate_debug_messenger_create_info();
        if (CreateDebugUtilsMessengerEXT(m_instance, &createInfo, nullptr, &m_debug_messenger) != VK_SUCCESS) {
            throw std::runtime_error("Vulkan: failed to set up debug messenger!");
        }
    }

    void Instance::create_surface()
    {
        if (glfwCreateWindowSurface(m_instance, m_window.lock()->glfw_window_ptr(), nullptr, &m_surface) !=
            VK_SUCCESS) {
            throw std::runtime_error("Vulkan: failed to create window surface!");
        }
    }

    void Instance::pick_physical_device()
    {
        std::uint32_t device_count = 0;
        vkEnumeratePhysicalDevices(m_instance, &device_count, nullptr);
        if (device_count == 0) {
            throw std::runtime_error("Vulkan: failed to find GPUs with Vulkan support!");
        }

        std::vector<VkPhysicalDevice> devices(device_count);
        vkEnumeratePhysicalDevices(m_instance, &device_count, devices.data());

        for (const auto& device: devices) {
            if (isDeviceSuitable(device)) {
                m_physical_device = device;
                break;
            }
        }

        if (m_physical_device == VK_NULL_HANDLE) {
            throw std::runtime_error("Vulkan: failed to find a suitable GPU!");
        }
    }

    bool Instance::isDeviceSuitable(VkPhysicalDevice device)
    {
#if 0
        VkPhysicalDeviceProperties deviceProperties;
        vkGetPhysicalDeviceProperties(device, &deviceProperties);
        VkPhysicalDeviceFeatures deviceFeatures;
        vkGetPhysicalDeviceFeatures(device, &deviceFeatures);
#endif
        const QueueFamilyIndices indices = findQueueFamilies(device);

        const bool extensionsSupported = checkDeviceExtensionSupport(device);

        bool swapChainAdequate = false;
        if (extensionsSupported) {
            const SwapChainSupportDetails swapChainSupport = querySwapChainSupport(device);
            swapChainAdequate = !swapChainSupport.formats.empty() && !swapChainSupport.presentModes.empty();
        }

        return indices.is_complete() && extensionsSupported && swapChainAdequate;
    }

    QueueFamilyIndices Instance::findQueueFamilies(const VkPhysicalDevice device)
    {
        QueueFamilyIndices indices;

        uint32_t queueFamilyCount = 0;
        vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);
        std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
        vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies.data());

        int i = 0;
        for (const auto& queueFamily: queueFamilies) {
            if (queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT) {
                indices.graphicsFamily = i;
            }

            VkBool32 presentSupport = false;
            vkGetPhysicalDeviceSurfaceSupportKHR(device, i, m_surface, &presentSupport);
            if (presentSupport) {
                indices.presentFamily = i;
            }

            if (indices.is_complete()) {
                break;
            }
            i++;
        }

        return indices;
    }

    void Instance::create_logical_device()
    {
        QueueFamilyIndices indices = findQueueFamilies(m_physical_device);

        std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
        std::set<std::uint32_t> uniqueQueueFamilies = {indices.graphicsFamily.value(), indices.presentFamily.value()};

        float queuePriority = 1.0f;
        for (std::uint32_t queueFamily: uniqueQueueFamilies) {
            VkDeviceQueueCreateInfo queueCreateInfo{};
            queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
            queueCreateInfo.queueFamilyIndex = queueFamily;
            queueCreateInfo.queueCount = 1;
            queueCreateInfo.pQueuePriorities = &queuePriority;
            queueCreateInfos.push_back(queueCreateInfo);
        }

        VkPhysicalDeviceFeatures deviceFeatures{};

        VkDeviceCreateInfo createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
        createInfo.queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos.size());
        createInfo.pQueueCreateInfos = queueCreateInfos.data();

        createInfo.pEnabledFeatures = &deviceFeatures;
        createInfo.enabledExtensionCount = static_cast<uint32_t>(DEVICE_EXTENSIONS.size());
        createInfo.ppEnabledExtensionNames = DEVICE_EXTENSIONS.data();

#ifdef NDEBUG
        createInfo.enabledLayerCount = 0;
#else
        // for backwards-compatibility
        createInfo.enabledLayerCount = static_cast<uint32_t>(VALIDATION_LAYERS.size());
        createInfo.ppEnabledLayerNames = VALIDATION_LAYERS.data();
#endif

        if (vkCreateDevice(m_physical_device, &createInfo, nullptr, &m_device) != VK_SUCCESS) {
            throw std::runtime_error("Vulkan: failed to create logical device!");
        }

        vkGetDeviceQueue(m_device, indices.graphicsFamily.value(), 0, &m_graphics_queue);
        vkGetDeviceQueue(m_device, indices.presentFamily.value(), 0, &m_present_queue);
    }

    bool Instance::checkDeviceExtensionSupport(const VkPhysicalDevice device)
    {
        std::uint32_t extensionCount;
        vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, nullptr);

        std::vector<VkExtensionProperties> availableExtensions(extensionCount);
        vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, availableExtensions.data());

        std::set<std::string> requiredExtensions(DEVICE_EXTENSIONS.begin(), DEVICE_EXTENSIONS.end());

        for (const auto& extension: availableExtensions) {
            requiredExtensions.erase(extension.extensionName);
        }

        return requiredExtensions.empty();
    }

    SwapChainSupportDetails Instance::querySwapChainSupport(const VkPhysicalDevice device)
    {
        SwapChainSupportDetails details;

        vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, m_surface, &details.capabilities);

        std::uint32_t formatCount;
        vkGetPhysicalDeviceSurfaceFormatsKHR(device, m_surface, &formatCount, nullptr);
        if (formatCount != 0) {
            details.formats.resize(formatCount);
            vkGetPhysicalDeviceSurfaceFormatsKHR(device, m_surface, &formatCount, details.formats.data());
        }

        std::uint32_t presentModeCount;
        vkGetPhysicalDeviceSurfacePresentModesKHR(device, m_surface, &presentModeCount, nullptr);
        if (presentModeCount != 0) {
            details.presentModes.resize(presentModeCount);
            vkGetPhysicalDeviceSurfacePresentModesKHR(device, m_surface, &presentModeCount,
                                                      details.presentModes.data());
        }

        return details;
    }

    VkSurfaceFormatKHR Instance::chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats)
    {
        for (const auto& availableFormat: availableFormats) {
            if (availableFormat.format == VK_FORMAT_B8G8R8A8_SRGB &&
                availableFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) {
                return availableFormat;
            }
        }

        return availableFormats[0];
    }

    VkPresentModeKHR Instance::chooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes)
    {
        for (const auto& availablePresentMode: availablePresentModes) {
            if (availablePresentMode == VK_PRESENT_MODE_MAILBOX_KHR) {
                // triple-buffering
                return availablePresentMode;
            }
        }

        return VK_PRESENT_MODE_FIFO_KHR; // v-sync
    }

    VkExtent2D Instance::chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities)
    {
        if (capabilities.currentExtent.width != std::numeric_limits<std::uint32_t>::max()) {
            return capabilities.currentExtent;
        }

        const int width = m_window.lock()->getPixelWidth();
        const int height = m_window.lock()->getPixelHeight();

        VkExtent2D actualExtent = {static_cast<std::uint32_t>(width), static_cast<std::uint32_t>(height)};

        actualExtent.width =
                std::clamp(actualExtent.width, capabilities.minImageExtent.width, capabilities.maxImageExtent.width);
        actualExtent.height =
                std::clamp(actualExtent.height, capabilities.minImageExtent.height, capabilities.maxImageExtent.height);

        return actualExtent;
    }

    void Instance::create_swap_chain()
    {
        const SwapChainSupportDetails swapChainSupport = querySwapChainSupport(m_physical_device);

        const VkSurfaceFormatKHR surfaceFormat = chooseSwapSurfaceFormat(swapChainSupport.formats);
        const VkPresentModeKHR presentMode = chooseSwapPresentMode(swapChainSupport.presentModes);
        const VkExtent2D extent = chooseSwapExtent(swapChainSupport.capabilities);

        std::uint32_t imageCount = swapChainSupport.capabilities.minImageCount + 1;
        if (swapChainSupport.capabilities.maxImageCount > 0 &&
            imageCount > swapChainSupport.capabilities.maxImageCount) {
            imageCount = swapChainSupport.capabilities.maxImageCount;
        }

        VkSwapchainCreateInfoKHR createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
        createInfo.surface = m_surface;

        createInfo.minImageCount = imageCount;
        createInfo.imageFormat = surfaceFormat.format;
        createInfo.imageColorSpace = surfaceFormat.colorSpace;
        createInfo.imageExtent = extent;
        createInfo.imageArrayLayers = 1;
        createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

        const QueueFamilyIndices indices = findQueueFamilies(m_physical_device);
        const std::uint32_t queueFamilyIndices[] = {indices.graphicsFamily.value(), indices.presentFamily.value()};
        if (indices.graphicsFamily != indices.presentFamily) {
            createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
            createInfo.queueFamilyIndexCount = 2;
            createInfo.pQueueFamilyIndices = queueFamilyIndices;
        } else {
            createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
            createInfo.queueFamilyIndexCount = 0; // Optional
            createInfo.pQueueFamilyIndices = nullptr; // Optional
        }

        createInfo.preTransform = swapChainSupport.capabilities.currentTransform; // no transformation
        createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR; // ignore alpha

        createInfo.presentMode = presentMode;
        createInfo.clipped = VK_TRUE; // clip pixels under overlapping windows

        createInfo.oldSwapchain = VK_NULL_HANDLE; // TODO: window resizing

        if (vkCreateSwapchainKHR(m_device, &createInfo, nullptr, &m_swap_chain) != VK_SUCCESS) {
            throw std::runtime_error("Vulkan: failed to create swap chain!");
        }

        vkGetSwapchainImagesKHR(m_device, m_swap_chain, &imageCount, nullptr);
        m_swap_chain_images.resize(imageCount);
        vkGetSwapchainImagesKHR(m_device, m_swap_chain, &imageCount, m_swap_chain_images.data());

        m_swap_chain_image_format = surfaceFormat.format;
        m_swap_chain_extent = extent;
    }

    void Instance::create_image_views()
    {
        std::vector<VkImageViewCreateInfo> create_infos;
        create_infos.resize(m_swap_chain_images.size());
        for (size_t i = 0; i < m_swap_chain_images.size(); i++) {
            VkImageViewCreateInfo create_info{};
            create_info.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
            create_info.image = m_swap_chain_images[i];

            create_info.viewType = VK_IMAGE_VIEW_TYPE_2D;
            create_info.format = m_swap_chain_image_format;

            create_info.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
            create_info.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
            create_info.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
            create_info.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;

            create_info.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
            create_info.subresourceRange.baseMipLevel = 0;
            create_info.subresourceRange.levelCount = 1;
            create_info.subresourceRange.baseArrayLayer = 0;
            create_info.subresourceRange.layerCount = 1;

            create_infos[i] = create_info;
        }
        m_swap_chain_image_view =
                m_store_image_views->create(this, create_infos, swap_chain_counter());
    }

    VkShaderModule Instance::createShaderModule(const std::vector<std::byte>& code)
    {
        VkShaderModuleCreateInfo createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
        createInfo.codeSize = code.size();
        createInfo.pCode = reinterpret_cast<const uint32_t*>(code.data());

        VkShaderModule shaderModule;
        if (vkCreateShaderModule(m_device, &createInfo, nullptr, &shaderModule) != VK_SUCCESS) {
            throw std::runtime_error("failed to create shader module!");
        }

        return shaderModule;
    }

    void Instance::create_swap_chain_render_pass()
    {
        VkAttachmentDescription colorAttachment{};
        colorAttachment.format = m_swap_chain_image_format;
        colorAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
        colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
        colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
        colorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
        colorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
        colorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
        colorAttachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

        VkAttachmentReference colorAttachmentRef{};
        colorAttachmentRef.attachment = 0;
        colorAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

        VkSubpassDescription subpass{};
        subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
        subpass.colorAttachmentCount = 1;
        subpass.pColorAttachments = &colorAttachmentRef;

        VkRenderPassCreateInfo renderPassInfo{};
        renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
        renderPassInfo.attachmentCount = 1;
        renderPassInfo.pAttachments = &colorAttachment;
        renderPassInfo.subpassCount = 1;
        renderPassInfo.pSubpasses = &subpass;

        VkSubpassDependency dependency{};
        dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
        dependency.dstSubpass = 0;
        dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
        dependency.srcAccessMask = 0;
        dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
        dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;

        renderPassInfo.dependencyCount = 1;
        renderPassInfo.pDependencies = &dependency;

        m_render_pass = m_store_render_passes->create(this, renderPassInfo);
    }

    void Instance::create_framebuffers()
    {
        FrameBufferCreator frame_buffer_factory(weak_from_this());

        std::array attachments = {m_swap_chain_image_view};
        m_swap_chain_frame_buffer = frame_buffer_factory.create_swap_chain(
                m_render_pass, attachments, m_swap_chain_extent.width, m_swap_chain_extent.height);
    }

    void Instance::create_command_pool()
    {
        const QueueFamilyIndices queueFamilyIndices = findQueueFamilies(m_physical_device);

        VkCommandPoolCreateInfo poolInfo{};
        poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
        poolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
        poolInfo.queueFamilyIndex = queueFamilyIndices.graphicsFamily.value();

        if (vkCreateCommandPool(m_device, &poolInfo, nullptr, &m_command_pool) != VK_SUCCESS) {
            throw std::runtime_error("Vulkan: failed to create command pool!");
        }
    }

    void Instance::create_command_buffers()
    {
        m_command_buffers.resize(MAX_FRAMES_IN_FLIGHT);

        VkCommandBufferAllocateInfo allocInfo{};
        allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
        allocInfo.commandPool = m_command_pool;
        allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
        allocInfo.commandBufferCount = static_cast<std::uint32_t>(m_command_buffers.size());

        if (vkAllocateCommandBuffers(m_device, &allocInfo, m_command_buffers.data()) != VK_SUCCESS) {
            throw std::runtime_error("Vulkan: failed to allocate command buffers!");
        }
    }

    void Instance::create_sync_objects()
    {
        m_image_available_semaphores.resize(MAX_FRAMES_IN_FLIGHT);
        m_render_finished_semaphores.resize(MAX_FRAMES_IN_FLIGHT);
        m_in_flight_fences.resize(MAX_FRAMES_IN_FLIGHT);

        VkSemaphoreCreateInfo semaphoreInfo{};
        semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

        VkFenceCreateInfo fenceInfo{};
        fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
        fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

        for (std::size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
            if (vkCreateSemaphore(m_device, &semaphoreInfo, nullptr, &m_image_available_semaphores[i]) != VK_SUCCESS ||
                vkCreateSemaphore(m_device, &semaphoreInfo, nullptr, &m_render_finished_semaphores[i]) != VK_SUCCESS ||
                vkCreateFence(m_device, &fenceInfo, nullptr, &m_in_flight_fences[i]) != VK_SUCCESS) {
                throw std::runtime_error("Vulkan: failed to create synchronization objects for a frame!");
            }
        }
    }

    void Instance::prepare_frame()
    {
        vkWaitForFences(m_device, 1, &m_in_flight_fences[m_current_frame_in_flight], VK_TRUE, UINT64_MAX);

        const VkResult result = vkAcquireNextImageKHR(m_device, m_swap_chain, UINT64_MAX,
                                                      m_image_available_semaphores[m_current_frame_in_flight],
                                                      VK_NULL_HANDLE, &m_current_swap_chain_image_index);
        if (result == VK_ERROR_OUT_OF_DATE_KHR) {
            recreateSwapChain();
            return;
        }

        if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) {
            throw std::runtime_error("failed to acquire swap chain image!");
        }

        // Only reset the fence if we are submitting work
        vkResetFences(m_device, 1, &m_in_flight_fences[m_current_frame_in_flight]);

        vkResetCommandBuffer(m_command_buffers[m_current_frame_in_flight], 0);
    }

    void Instance::present_frame()
    {
        VkSubmitInfo submitInfo{};
        submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

        const VkSemaphore waitSemaphores[] = {m_image_available_semaphores[m_current_frame_in_flight]};
        constexpr VkPipelineStageFlags waitStages[] = {VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};
        submitInfo.waitSemaphoreCount = 1;
        submitInfo.pWaitSemaphores = waitSemaphores;
        submitInfo.pWaitDstStageMask = waitStages;

        submitInfo.commandBufferCount = 1;
        submitInfo.pCommandBuffers = &m_command_buffers[m_current_frame_in_flight];

        const VkSemaphore signalSemaphores[] = {m_render_finished_semaphores[m_current_frame_in_flight]};
        submitInfo.signalSemaphoreCount = 1;
        submitInfo.pSignalSemaphores = signalSemaphores;

        if (vkQueueSubmit(m_graphics_queue, 1, &submitInfo, m_in_flight_fences[m_current_frame_in_flight]) !=
            VK_SUCCESS) {
            throw std::runtime_error("Vulkan: failed to submit draw command buffer!");
        }

        VkPresentInfoKHR presentInfo{};
        presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;

        presentInfo.waitSemaphoreCount = 1;
        presentInfo.pWaitSemaphores = signalSemaphores;

        const VkSwapchainKHR swapChains[] = {m_swap_chain};
        presentInfo.swapchainCount = 1;
        presentInfo.pSwapchains = swapChains;
        presentInfo.pImageIndices = &m_current_swap_chain_image_index;
        presentInfo.pResults = nullptr; // Optional

        const VkResult result = vkQueuePresentKHR(m_present_queue, &presentInfo);
        if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR || m_framebuffer_resized) {
            m_framebuffer_resized = false;
            recreateSwapChain();
        } else if (result != VK_SUCCESS) {
            throw std::runtime_error("Vulkan: failed to present swap chain image!");
        }

        m_current_frame_in_flight = (m_current_frame_in_flight + 1) % MAX_FRAMES_IN_FLIGHT;
    }

    void Instance::flush_resources()
    {
        vkDeviceWaitIdle(m_device);
        cleanupSwapChain();
    }

    FrameCounter Instance::swap_chain_counter() const
    {
        return {
                .curent_frame_index = &m_current_swap_chain_image_index,
                .max_frame_index = static_cast<unsigned int>(m_swap_chain_images.size()),
        };
    }

    FrameCounter Instance::frames_in_flight_counter() const
    {
        return {
                .curent_frame_index = &m_current_frame_in_flight,
                .max_frame_index = MAX_FRAMES_IN_FLIGHT,
        };
    }

    FrameCounter Instance::static_counter() const
    {
        return {
                .curent_frame_index = &m_current_frame_static,
                .max_frame_index = 1,
        };
    }

    VkDevice Instance::device() const
    {
        return m_device;
    }

    VkPhysicalDevice Instance::physical_device() const
    {
        return m_physical_device;
    }

    VkQueue Instance::graphics_queue() const
    {
        return m_graphics_queue;
    }

    FrameBuffer& Instance::swap_chain_frame_buffer() const
    {
        return m_swap_chain_frame_buffer.get<FrameBuffer>();
    }

    const RenderPassHandle& Instance::swap_chain_render_pass() const
    {
        return m_render_pass;
    }

    VkCommandBuffer Instance::command_buffer_for_frame() const
    {
        return m_command_buffers[m_current_frame_in_flight];
    }

    RenderPassStore& Instance::store_render_passes()
    {
        return *m_store_render_passes;
    }

    FrameBufferStore& Instance::store_frame_buffers()
    {
        return *m_store_frame_buffers;
    }

    PipelineStore& Instance::store_pipelines()
    {
        return *m_store_pipelines;
    }

    VertexBufferStore& Instance::store_vertex_buffers()
    {
        return *m_store_vertex_buffers;
    }

    IndexBufferStore& Instance::store_index_buffers()
    {
        return *m_store_index_buffers;
    }

    UniformBufferStore& Instance::store_uniform_buffers()
    {
        return *m_store_uniform_buffers;
    }

    DrawableStore& Instance::store_drawables()
    {
        return *m_store_drawables;
    }

    Texture2DStore& Instance::store_textures()
    {
        return *m_store_textures;
    }

    DescriptorSetLayoutStore& Instance::store_descriptor_set_layouts()
    {
        return *m_store_descriptor_set_layouts;
    }

    DescriptorAllocator& Instance::descriptor_allocator()
    {
        return m_descriptor_allocator.value();
    }

    VkCommandPool Instance::command_pool() const
    {
        return m_command_pool;
    }

    std::uint32_t Instance::find_memory_type(const std::uint32_t typeFilter, const VkMemoryPropertyFlags properties)
    {
        VkPhysicalDeviceMemoryProperties memProperties;
        vkGetPhysicalDeviceMemoryProperties(m_physical_device, &memProperties);

        for (std::uint32_t i = 0; i < memProperties.memoryTypeCount; i++) {
            if ((typeFilter & (1 << i)) && (memProperties.memoryTypes[i].propertyFlags & properties) == properties) {
                return i;
            }
        }

        throw std::runtime_error("Vulkan: failed to find suitable memory type!");
    }

    void Instance::create_buffer(const VkDeviceSize size, const VkBufferUsageFlags usage,
                                 const VkMemoryPropertyFlags properties, VkBuffer& buffer,
                                 VkDeviceMemory& buffer_memory)
    {
        VkBufferCreateInfo create_info{};
        create_info.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
        create_info.size = size;
        create_info.usage = usage;
        create_info.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
        create_info.flags = 0;
        if (vkCreateBuffer(m_device, &create_info, nullptr, &buffer) != VK_SUCCESS) {
            throw std::runtime_error("Vulkan: failed to create vertex buffer!");
        }

        VkMemoryRequirements memory_requirements;
        vkGetBufferMemoryRequirements(m_device, buffer, &memory_requirements);

        // TODO: don't call vkAllocateMemory for every buffer; implement an allocator or use VulkanMemoryAllocator
        VkMemoryAllocateInfo alloc_info{};
        alloc_info.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
        alloc_info.allocationSize = memory_requirements.size;
        alloc_info.memoryTypeIndex = find_memory_type(memory_requirements.memoryTypeBits, properties);
        if (vkAllocateMemory(m_device, &alloc_info, nullptr, &buffer_memory) != VK_SUCCESS) {
            throw std::runtime_error("Vulkan: failed to allocate vertex buffer memory!");
        }

        vkBindBufferMemory(m_device, buffer, buffer_memory, 0);
    }

    void Instance::copy_buffer(const VkBuffer source, const VkBuffer destination, const VkDeviceSize size)
    {
        const VkCommandBuffer command_buffer = begin_one_time_command_buffer();

        VkBufferCopy copy_region{};
        copy_region.srcOffset = 0; // Optional
        copy_region.dstOffset = 0; // Optional
        copy_region.size = size;
        vkCmdCopyBuffer(command_buffer, source, destination, 1, &copy_region);

        end_one_time_command_buffer(command_buffer);
    }

    void Instance::create_image(const std::uint32_t width, const std::uint32_t height, const VkFormat format,
                                const VkImageTiling tiling, const VkImageUsageFlags usage,
                                const VkMemoryPropertyFlags properties, VkImage& image, VkDeviceMemory& image_memory)
    {
        VkImageCreateInfo create_info{};
        create_info.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
        create_info.imageType = VK_IMAGE_TYPE_2D;
        create_info.extent.width = width;
        create_info.extent.height = height;
        create_info.extent.depth = 1;
        create_info.mipLevels = 1;
        create_info.arrayLayers = 1;
        create_info.format = format;
        create_info.tiling = tiling;
        create_info.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
        create_info.usage = usage;
        create_info.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
        create_info.samples = VK_SAMPLE_COUNT_1_BIT;
        create_info.flags = 0; // Optional

        if (vkCreateImage(m_device, &create_info, nullptr, &image) != VK_SUCCESS) {
            throw std::runtime_error("failed to create image!");
        }

        VkMemoryRequirements memory_requirements;
        vkGetImageMemoryRequirements(m_device, image, &memory_requirements);

        // TODO: don't call vkAllocateMemory for every buffer; implement an allocator or use VulkanMemoryAllocator
        VkMemoryAllocateInfo alloc_info{};
        alloc_info.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
        alloc_info.allocationSize = memory_requirements.size;
        alloc_info.memoryTypeIndex = find_memory_type(memory_requirements.memoryTypeBits, properties);

        if (vkAllocateMemory(m_device, &alloc_info, nullptr, &image_memory) != VK_SUCCESS) {
            throw std::runtime_error("failed to allocate image memory!");
        }

        vkBindImageMemory(m_device, image, image_memory, 0);
    }

    VkCommandBuffer Instance::begin_one_time_command_buffer()
    {
        VkCommandBufferAllocateInfo alloc_info{};
        alloc_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
        alloc_info.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
        alloc_info.commandPool = m_command_pool;
        alloc_info.commandBufferCount = 1;

        VkCommandBuffer command_buffer;
        // TODO: don't call vkAllocateMemory for every buffer; implement an allocator or use VulkanMemoryAllocator
        vkAllocateCommandBuffers(m_device, &alloc_info, &command_buffer);

        VkCommandBufferBeginInfo beginInfo{};
        beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
        beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

        vkBeginCommandBuffer(command_buffer, &beginInfo);

        return command_buffer;
    }

    void Instance::end_one_time_command_buffer(const VkCommandBuffer command_buffer)
    {
        vkEndCommandBuffer(command_buffer);

        VkSubmitInfo submit_info{};
        submit_info.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
        submit_info.commandBufferCount = 1;
        submit_info.pCommandBuffers = &command_buffer;
        // we could use fences here and schedule multiple buffer operations together
        vkQueueSubmit(m_graphics_queue, 1, &submit_info, VK_NULL_HANDLE);
        vkQueueWaitIdle(m_graphics_queue);

        vkFreeCommandBuffers(m_device, m_command_pool, 1, &command_buffer);
    }

    void Instance::transition_image_layout(const VkImage image, VkFormat, const VkImageLayout old_layout,
                                           const VkImageLayout new_layout)
    {
        const VkCommandBuffer command_buffer = begin_one_time_command_buffer();

        VkImageMemoryBarrier barrier{};
        barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
        barrier.oldLayout = old_layout;
        barrier.newLayout = new_layout;
        barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
        barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
        barrier.image = image;
        barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        barrier.subresourceRange.baseMipLevel = 0;
        barrier.subresourceRange.levelCount = 1;
        barrier.subresourceRange.baseArrayLayer = 0;
        barrier.subresourceRange.layerCount = 1;

        VkPipelineStageFlags source_stage;
        VkPipelineStageFlags destination_stage;
        if (old_layout == VK_IMAGE_LAYOUT_UNDEFINED && new_layout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL) {
            barrier.srcAccessMask = 0;
            barrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;

            source_stage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
            destination_stage = VK_PIPELINE_STAGE_TRANSFER_BIT;
        } else if (old_layout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL &&
                   new_layout == VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL) {
            barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
            barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

            source_stage = VK_PIPELINE_STAGE_TRANSFER_BIT;
            destination_stage = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
        } else {
            throw std::invalid_argument("unsupported layout transition!");
        }

        vkCmdPipelineBarrier(command_buffer, source_stage, destination_stage, 0, 0, nullptr, 0, nullptr, 1, &barrier);

        end_one_time_command_buffer(command_buffer);
    }

    void Instance::copy_buffer_to_image(const VkBuffer buffer, const VkImage image, const std::uint32_t width,
                                        const std::uint32_t height)
    {
        const VkCommandBuffer command_buffer = begin_one_time_command_buffer();

        VkBufferImageCopy region{};
        region.bufferOffset = 0;
        region.bufferRowLength = 0;
        region.bufferImageHeight = 0;
        region.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        region.imageSubresource.mipLevel = 0;
        region.imageSubresource.baseArrayLayer = 0;
        region.imageSubresource.layerCount = 1;
        region.imageOffset = {0, 0, 0};
        region.imageExtent = {width, height, 1};

        vkCmdCopyBufferToImage(command_buffer, buffer, image, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &region);

        end_one_time_command_buffer(command_buffer);
    }

    void Instance::cleanupSwapChain()
    {
        m_swap_chain_frame_buffer.reset();

        m_swap_chain_image_view.reset();

        vkDestroySwapchainKHR(m_device, m_swap_chain, nullptr);
    }

    void Instance::recreateSwapChain()
    {
        const auto window = m_window.lock();
        int width = window->getPixelWidth();
        int height = window->getPixelHeight();
        while (width == 0 || height == 0) {
            if (window->shouldDestroy())
                return;
            width = window->getPixelWidth();
            height = window->getPixelHeight();
            glfwWaitEvents(); // TODO
        }

        vkDeviceWaitIdle(m_device);

        cleanupSwapChain();

        create_swap_chain();
        create_image_views();
        create_framebuffers();
    }

    void Instance::create_descriptor_allocator()
    {
        std::vector<DescriptorAllocator::PoolSizeRatio> sizes = {{VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1}};
        m_descriptor_allocator = DescriptorAllocator(this, 10, sizes);
    }
}
