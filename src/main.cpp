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
        VkInstance instance;
        
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
            pickPhysicalDevice(&instance, &physicalDevice);

            // printf("Hello world");

            VkPhysicalDeviceProperties props = getPhysicalDeviceProperties(&physicalDevice);            
            printPhysicalDeviceInfo(props);
        }

        void mainLoop() {
            while (!glfwWindowShouldClose(window)) {
                glfwPollEvents();
            }
        }

        void cleanup() {
            vkDestroyInstance(instance, nullptr);
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
