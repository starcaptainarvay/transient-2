#include "vulkan_utils.h"

#include <stdio.h>
#include <stdlib.h>
#include <GLFW/glfw3.h>
#include <vulkan/vulkan.h>

VkSurfaceKHR createSurface(VkInstance instance, GLFWwindow* window) {
    VkSurfaceKHR surface;
    if (glfwCreateWindowSurface(instance, window, NULL, &surface) != VK_SUCCESS) {
        fprintf(stderr, "Failed to create window surface\n");
        exit(EXIT_FAILURE);
    }
    return surface;
}

VkInstance createInstance() {
    // Specify application info
    VkApplicationInfo appInfo = { VK_STRUCTURE_TYPE_APPLICATION_INFO };
    appInfo.pApplicationName = "Transient";
    appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.pEngineName = "No Engine";
    appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.apiVersion = VK_API_VERSION_1_0;

    // Specify instance creation info
    VkInstanceCreateInfo createInfo = { VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO };
    createInfo.pApplicationInfo = &appInfo;

    // Get required instance extensions
    uint32_t glfwExtensionCount = 0;
    const char** glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);
    createInfo.enabledExtensionCount = glfwExtensionCount;
    createInfo.ppEnabledExtensionNames = glfwExtensions;

    // Create Vulkan instance
    VkInstance instance;
    if (vkCreateInstance(&createInfo, NULL, &instance) != VK_SUCCESS) {
        fprintf(stderr, "Failed to create Vulkan instance\n");
        exit(EXIT_FAILURE);
    }

    return instance;
}

void initVulkan();
void createGraphicsPipeline();
void createRenderPass();