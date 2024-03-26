#ifndef VULKAN_WINDOW_H
#define VULKAN_WINDOW_H

#define GLFW_INCLUDE_VULKAN
#define EXIT_FAILURE 1

#include <GLFW/glfw3.h>
#include <vulkan/vulkan.h>

typedef struct {
    GLFWwindow* window;
    VkInstance instance;
    VkSurfaceKHR surface;
} TransientWindowState;

TransientWindowState* createTransientWindow();
void cleanupTransientWindow(TransientWindowState* state);

// Function prototypes related to Vulkan utilities
void initVulkan();
void createGraphicsPipeline();
void createRenderPass();

VkSurfaceKHR createSurface(VkInstance instance, GLFWwindow* window);
VkInstance createInstance();

#endif
