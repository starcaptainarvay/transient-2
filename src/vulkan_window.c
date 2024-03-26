#include "vulkan_window.h"

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

TransientWindowState* createTransientWindow() {
    if (!glfwInit()) {
        fprintf(stderr, "Failed to initialize GLFW\n");
        exit(EXIT_FAILURE);
    }

    GLFWmonitor* primaryMonitor = glfwGetPrimaryMonitor();
    if (!primaryMonitor) {
        fprintf(stderr, "Unable to get primary monitor.");
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    const GLFWvidmode* videoMode = glfwGetVideoMode(primaryMonitor);
    if (!videoMode) {
        fprintf(stderr, "Unable to get video mode.");
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    /*
        Disable OpenGL binding for GLFW window.
        See: https://stackoverflow.com/questions/73991660/vulkan-raii-got-vk-error-native-window-in-use-khr-when-trying-to-create-vksurfa
    */
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API); 

    // Create GLFW window
    GLFWwindow* window = glfwCreateWindow(videoMode->width, videoMode->height, "Transient", primaryMonitor, NULL);
    if (!window) {
        fprintf(stderr, "Failed to create GLFW window\n");
        glfwTerminate();
        
        exit(EXIT_FAILURE);
    }

    // Create Vulkan instance
    VkInstance instance = createInstance();

    // Create Vulkan surface
    VkSurfaceKHR surface = createSurface(instance, window);

    TransientWindowState* state = malloc(sizeof(TransientWindowState));
    state->window   = window;
    state->instance = instance;
    state->surface  = surface;

    return state;
}

void cleanupTransientWindow(TransientWindowState *state) {
    vkDestroySurfaceKHR(state->instance, state->surface, NULL);
    vkDestroyInstance(state->instance, NULL);
    glfwDestroyWindow(state->window);
    glfwTerminate();
}

void initVulkan();
void createGraphicsPipeline();
void createRenderPass();