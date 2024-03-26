#include "obrien.h"
#include "vulkan_utils.h"

#define GLFW_INCLUDE_VULKAN
#define EXIT_FAILURE -1

#include <GLFW/glfw3.h>
#include <vulkan/vulkan.h>
#include <stdio.h>
#include <stdlib.h>

void runApplication() {
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

    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();

        // Rendering with Vulkan

        // Present rendering result
    }

    // Clean up
    vkDestroySurfaceKHR(instance, surface, NULL);
    vkDestroyInstance(instance, NULL);
    glfwDestroyWindow(window);
    glfwTerminate();
}

int main() {
    runApplication();
    return 0;
}