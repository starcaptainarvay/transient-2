#include <vulkan/vulkan.h>
#include <GLFW/glfw3.h>
#include <stdexcept>

#include "vulkan_pipeline_utils.h"

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
