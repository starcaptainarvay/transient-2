#ifndef VULKAN_UTILS_H
#define VULKAN_UTILS_H

#define GLFW_INCLUDE_VULKAN
#define EXIT_FAILURE 1

#include <GLFW/glfw3.h>
#include <vulkan/vulkan.h>

// Function prototypes related to Vulkan utilities
void initVulkan();
void createGraphicsPipeline();
void createRenderPass();

VkSurfaceKHR createSurface(VkInstance instance, GLFWwindow* window);
VkInstance createInstance();

#endif
