#include <vulkan/vulkan.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <stdexcept>
#include <cstdlib>

#include "vulkan_pipeline_utils.h"
#include "vulkan_debug_utils.h"

class TransientApplication {
    public:
        void run() {
            printf("Initializing window.\n");
            initWindow();
            printf("Initializing Vulkan.\n");
            initVulkan();
            printf("Initializing Transient.\n");
            mainLoop();
            printf("Cleaning up Transient.");
            cleanup();
        }

    private:
        GLFWwindow* window;
        VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
        VkDevice device = VK_NULL_HANDLE;
        VkQueue graphicsQueue = VK_NULL_HANDLE;
        VkQueue presentQueue = VK_NULL_HANDLE;
        VkInstance instance;
        VkSurfaceKHR surface;

        void createSurface() {
            if (glfwCreateWindowSurface(instance, window, nullptr, &surface) != VK_SUCCESS) {
                throw std::runtime_error("failed to create window surface!");
            }
        }

        
        void initWindow() {
            glfwInit();

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

            glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
            glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

            window = glfwCreateWindow(videoMode->width, videoMode->height, "Transient", primaryMonitor, nullptr);
        }

        void initVulkan() {
            createInstance(&instance);
            createSurface();

            pickPhysicalDevice(&instance, &physicalDevice);
            printPhysicalDeviceInfo(&physicalDevice);
            QueueFamilyIndices indices = createLogicalDevice(&device, &physicalDevice, &surface);

            // Initialize graphics queue
            vkGetDeviceQueue(device, indices.graphicsFamily.value(), 0, &graphicsQueue);
            vkGetDeviceQueue(device, indices.presentFamily.value(), 0, &presentQueue);

            // VkPhysicalDeviceProperties props = getPhysicalDeviceProperties(&physicalDevice);

            // QueueFamilyIndices queueIndices = findQueueFamilies(physicalDevice);
            // printf("Graphics family index: %d\n", queueIndices.graphicsFamily);
        }

        void mainLoop() {
            while (!glfwWindowShouldClose(window)) {
                glfwPollEvents();
            }
        }

        void cleanup() {
            vkDestroySurfaceKHR(instance, surface, nullptr);
            vkDestroyInstance(instance, nullptr);
            vkDestroyDevice(device, nullptr);
            glfwDestroyWindow(window);

            glfwTerminate();
        }
};

int main() {
    TransientApplication app;

    try {
        app.run();
    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
