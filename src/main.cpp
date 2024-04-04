#include <vulkan/vulkan.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <stdexcept>
#include <cstdlib>

class TransientApplication {
    GLFWwindow* window;

    public:
        void run() {
            initWindow();
            initVulkan();
            mainLoop();
            cleanup();
        }

    private:
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

        }

        void mainLoop() {
            while (!glfwWindowShouldClose(window)) {
                glfwPollEvents();
            }
        }

        void cleanup() {
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
