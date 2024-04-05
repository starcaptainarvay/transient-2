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

QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device) {
    QueueFamilyIndices indices;

    uint32_t queueFamilyCount = 0;
    vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);

    std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
    vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies.data());

    int i = 0;
    for (const auto& queueFamily: queueFamilies) {
        printf("Queue families[%d] flags: %d\n", i, queueFamily.queueFlags);

        if (queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT) {
            indices.graphicsFamily = i;
        }

        if (indices.isComplete()) {
            break;
        }

        i++;
    }

    return indices;
}

QueueFamilyIndices createLogicalDevice(VkDevice *pDevice, VkPhysicalDevice *pPhysicalDevice) {
    QueueFamilyIndices indices = findQueueFamilies(*pPhysicalDevice);

    VkDeviceQueueCreateInfo queueCreateInfo{};
    queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
    queueCreateInfo.queueFamilyIndex = indices.graphicsFamily.value();
    queueCreateInfo.queueCount = 1;

    VkPhysicalDeviceFeatures deviceFeatures{};
    vkGetPhysicalDeviceFeatures(*pPhysicalDevice, &deviceFeatures);

    float queuePriority = 1.0f;
    queueCreateInfo.pQueuePriorities = &queuePriority;

    VkDeviceCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    createInfo.pQueueCreateInfos = &queueCreateInfo;
    createInfo.queueCreateInfoCount = 1;

    createInfo.pEnabledFeatures = &deviceFeatures; // enable features offered by physical device on logical device

    if (vkCreateDevice(*pPhysicalDevice, &createInfo, nullptr, pDevice) != VK_SUCCESS) {
        throw std::runtime_error("failed to create logical device!");
    }

    createInfo.enabledExtensionCount = 0;

    return indices;   
}