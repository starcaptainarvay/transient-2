#include "obrien.h"
#include "vulkan_window.h"

#include <GLFW/glfw3.h>
#include <stdio.h>
#include <stdlib.h>

void runApplication() {
    // Create GLFW window
    TransientWindowState* state = createTransientWindow();

    while (!glfwWindowShouldClose(state->window)) {
        glfwPollEvents();

        // Rendering with Vulkan

        // Present rendering result
    }

    // Clean up
    cleanupTransientWindow(state);
    free(state);
}

int main() {
    runApplication();
    return 0;
}