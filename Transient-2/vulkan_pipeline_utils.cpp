#include <vulkan/vulkan.h>
#include <GLFW/glfw3.h>

#include "vulkan_pipeline_utils.h"
#include "vulkan_debug_utils.h"

#include <stdexcept>
#include <vector>
#include <set>
#include <optional>

void createInstance(VkInstance* pInstance) {
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

bool checkDeviceExtensionSupport(VkPhysicalDevice device) {
    uint32_t extensionCount;
    vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, nullptr);

    std::vector<VkExtensionProperties> availableExtensions(extensionCount);
    vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, availableExtensions.data());

    std::set<std::string> requiredExtensions(TRANSIENT_REQUIRED_DEVICE_EXTENSIONS.begin(), TRANSIENT_REQUIRED_DEVICE_EXTENSIONS.end());

    for (const auto& extension : availableExtensions) {
        requiredExtensions.erase(extension.extensionName);
    }

    return requiredExtensions.empty();
}

SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice device, VkSurfaceKHR surface) {
    SwapChainSupportDetails details;

    vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, surface, &details.capabilities);

    uint32_t formatCount;
    vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &formatCount, nullptr);

    if (formatCount != 0) {
        details.formats.resize(formatCount);
        vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &formatCount, details.formats.data());
    }

    uint32_t presentModeCount;
    vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &presentModeCount, nullptr);

    if (presentModeCount != 0) {
        details.presentModes.resize(presentModeCount);
        vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &presentModeCount, details.presentModes.data());
    }

    return details;
}

bool isDeviceSuitable(VkPhysicalDevice device, VkSurfaceKHR surface) {
    QueueFamilyIndices indices = findQueueFamilies(device, surface);

    bool extensionsSupported = checkDeviceExtensionSupport(device);

    bool swapChainAdequate = false;
    if (extensionsSupported) {
        SwapChainSupportDetails swapChainSupport = querySwapChainSupport(device, surface);
        swapChainAdequate = !swapChainSupport.formats.empty() && !swapChainSupport.presentModes.empty();
        printSwapChainSupportDetails(swapChainSupport);
    }

    return indices.isComplete() && extensionsSupported && swapChainAdequate;
}

void pickPhysicalDevice(VkInstance* pInstance, VkPhysicalDevice* pDevice, VkSurfaceKHR* surface) {
    uint32_t deviceCount = 0;
    vkEnumeratePhysicalDevices(*pInstance, &deviceCount, nullptr);

    if (deviceCount == 0) {
        throw std::runtime_error("failed to find GPUs with Vulkan support!");
    }

    std::vector<VkPhysicalDevice> devices(deviceCount);
    vkEnumeratePhysicalDevices(*pInstance, &deviceCount, devices.data());

    for (const auto& device : devices) {
        if (isDeviceSuitable(device, *surface)) {
            *pDevice = device;
            break;
        }
    }

    if (*pDevice == VK_NULL_HANDLE) {
        throw std::runtime_error("failed to find a suitable GPU!");
    }
}

QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device, VkSurfaceKHR surface) {
    QueueFamilyIndices indices;

    uint32_t queueFamilyCount = 0;
    vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);

    std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
    vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies.data());

    int i = 0;
    for (const auto& queueFamily : queueFamilies) {
        printf("Queue families[%d] flags: %d\n", i, queueFamily.queueFlags);

        if (queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT) {
            indices.graphicsFamily = i;
            printf("Queue families[%d] offers Graphics\n", i);
        }

        // else {
        VkBool32 presentSupport = false;
        vkGetPhysicalDeviceSurfaceSupportKHR(device, i, surface, &presentSupport);

        if (presentSupport) {
            indices.presentFamily = i;
            printf("Queue families[%d] offers Presentation\n", i);
        }
        // }

        if (indices.isComplete()) {
            break;
        }

        i++;
    }

    return indices;
}

QueueFamilyIndices createLogicalDevice(VkDevice* pDevice, VkPhysicalDevice* pPhysicalDevice, VkSurfaceKHR* surface) {
    QueueFamilyIndices indices = findQueueFamilies(*pPhysicalDevice, *surface);

    std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
    std::set<uint32_t> uniqueQueueFamilies = { indices.graphicsFamily.value(), indices.presentFamily.value() };

    float queuePriority = 1.0f;
    for (uint32_t queueFamily : uniqueQueueFamilies) {
        printf("Generating VkQueue Create Info for %d\n", queueFamily);
        VkDeviceQueueCreateInfo queueCreateInfo{};
        queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
        queueCreateInfo.queueFamilyIndex = queueFamily;
        queueCreateInfo.queueCount = 1;
        queueCreateInfo.pQueuePriorities = &queuePriority;
        queueCreateInfos.push_back(queueCreateInfo);
    }

    VkPhysicalDeviceFeatures deviceFeatures{};
    vkGetPhysicalDeviceFeatures(*pPhysicalDevice, &deviceFeatures);

    VkDeviceCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    createInfo.pQueueCreateInfos = queueCreateInfos.data();
    createInfo.queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos.size());

    createInfo.pEnabledFeatures = &deviceFeatures; // enable features offered by physical device on logical device

    if (vkCreateDevice(*pPhysicalDevice, &createInfo, nullptr, pDevice) != VK_SUCCESS) {
        throw std::runtime_error("failed to create logical device!");
    }

    createInfo.enabledExtensionCount = 0;

    return indices;
}