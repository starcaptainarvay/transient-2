#include "obrien.h"
#include "video_manager.h"
#include "vulkan_utils.h"

void runApplication() {
    initVideoManager();
    loadVideo("SilentAssembly_3-8.mp4");
    initVulkan();
    createRenderPass();
    createGraphicsPipeline();
    // renderFrame();
}

int main() {
    runApplication();
    return 0;
}