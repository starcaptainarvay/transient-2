#include <vulkan/vulkan.h>
#include <GLFW/glfw3.h>

#include "vulkan_pipeline_utils.h"

#include <stdexcept>
#include <vector>
#include <optional>

void createInstance(VkInstance *pInstance) {
    VkApplicationInfo appInfo{};
    appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    appInfo.pApplicationName = "Transient";
    appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.pEngineName = "No Engine";
    appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.apiVersion = VK_API_VERSION_1_0;

    VkInstanceCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    createInfo.pApplicationInfo = &appInfo;

    uint32_t glfwExtensionCount = 0;
    const char** glfwExtensions;

    glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

    createInfo.enabledExtensionCount = glfwExtensionCount;
    createInfo.ppEnabledExtensionNames = glfwExtensions;

    createInfo.enabledLayerCount = 0;

    // VkResult result = vkCreateInstance(&createInfo, nullptr, pInstance);
    if (vkCreateInstance(&createInfo, nullptr, pInstance) != VK_SUCCESS) {
        throw std::runtime_error("failed to create instance!");
    }
}

bool isDeviceSuitable(VkPhysicalDevice device) {
    (void) device; // suppress unused param
    return true;
}

void pickPhysicalDevice(VkInstance *pInstance, VkPhysicalDevice *pDevice) {
    uint32_t deviceCount = 0;
    vkEnumeratePhysicalDevices(*pInstance, &deviceCount, nullptr);

    if (deviceCount == 0) {
        throw std::runtime_error("failed to find GPUs with Vulkan support!");
    }

    std::vector<VkPhysicalDevice> devices(deviceCount);
    vkEnumeratePhysicalDevices(*pInstance, &deviceCount, devices.data());

    for (const auto& device: devices) {
        if (isDeviceSuitable(device)) {
            *pDevice = device;
            break;
        }
    }

    if (*pDevice == VK_NULL_HANDLE) {
        throw std::runtime_error("failed to find a suitable GPU!");
    }
}

struct QueueFamilyIndices {
    std::optional<uint32_t> graphicsFamily;
};

QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device) {
    QueueFamilyIndices indices;
    
    return indices;
}