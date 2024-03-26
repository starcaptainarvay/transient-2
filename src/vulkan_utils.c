#include "vulkan_utils.h"

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <vulkan/vulkan.h>
#include <string.h>

VkInstance instance;
VkPhysicalDevice physicalDevice;
VkDevice device;
VkQueue graphicsQueue;
VkCommandPool commandPool;
VkSurfaceKHR surface;
VkSwapchainKHR swapchain;
VkFormat swapchainImageFormat;
VkExtent2D swapchainExtent;
VkImage* swapchainImages;
VkImageView* swapchainImageViews;
VkRenderPass renderPass;
VkPipelineLayout pipelineLayout;
VkPipeline graphicsPipeline;
VkFramebuffer* framebuffers;
VkCommandBuffer* commandBuffers;
VkSemaphore imageAvailableSemaphore;
VkSemaphore renderFinishedSemaphore;

VkShaderModule vertexShaderModule;
VkShaderModule fragmentShaderModule;

char* loadShaderCode(const char* filename) {
    FILE* file = fopen(filename, "rb");
    if (!file) {
        fprintf(stderr, "Failed to open shader file: %s\n", filename);
        exit(EXIT_FAILURE);
    }
    fseek(file, 0, SEEK_END);
    long fileSize = ftell(file);
    fseek(file, 0, SEEK_SET);
    char* buffer = (char*)malloc(fileSize + 1);
    if (!buffer) {
        fprintf(stderr, "Failed to allocate memory for shader file: %s\n", filename);
        exit(EXIT_FAILURE);
    }
    fread(buffer, 1, fileSize, file);
    fclose(file);
    buffer[fileSize] = '\0';
    return buffer;
}

void initVulkan() {
    // Create Vulkan instance
    VkApplicationInfo appInfo = {};
    appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    appInfo.pApplicationName = "Vulkan Video Player";
    appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.pEngineName = "No Engine";
    appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.apiVersion = VK_API_VERSION_1_0;

    VkInstanceCreateInfo createInfo = {};
    createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    createInfo.pApplicationInfo = &appInfo;

    if (vkCreateInstance(&createInfo, NULL, &instance) != VK_SUCCESS) {
        fprintf(stderr, "Failed to create Vulkan instance!\n");
        exit(EXIT_FAILURE);
    }

    // Get physical device
    uint32_t deviceCount = 0;
    vkEnumeratePhysicalDevices(instance, &deviceCount, NULL);
    if (deviceCount == 0) {
        fprintf(stderr, "Failed to find GPUs with Vulkan support!\n");
        exit(EXIT_FAILURE);
    }

    VkPhysicalDevice* devices = malloc(deviceCount * sizeof(VkPhysicalDevice));
    vkEnumeratePhysicalDevices(instance, &deviceCount, devices);
    physicalDevice = devices[0]; // Select the first physical device for simplicity

    free(devices);

    // Create logical device and graphics queue
    float queuePriority = 1.0f;
    VkDeviceQueueCreateInfo queueCreateInfo = {};
    queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
    queueCreateInfo.queueFamilyIndex = 0; // Assume queue family index 0 supports graphics
    queueCreateInfo.queueCount = 1;
    queueCreateInfo.pQueuePriorities = &queuePriority;

    VkDeviceCreateInfo deviceCreateInfo = {};
    deviceCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    deviceCreateInfo.queueCreateInfoCount = 1;
    deviceCreateInfo.pQueueCreateInfos = &queueCreateInfo;

    if (vkCreateDevice(physicalDevice, &deviceCreateInfo, NULL, &device) != VK_SUCCESS) {
        fprintf(stderr, "Failed to create logical device!\n");
        exit(EXIT_FAILURE);
    }

    vkGetDeviceQueue(device, 0, 0, &graphicsQueue);

    printf("Vulkan initialization complete.\n");
}

void createRenderPass() {
    VkAttachmentDescription colorAttachment = {};
    colorAttachment.format = VK_FORMAT_B8G8R8A8_UNORM;
    colorAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
    colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
    colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
    colorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    colorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    colorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    colorAttachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

    VkAttachmentReference colorAttachmentRef = {};
    colorAttachmentRef.attachment = 0;
    colorAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

    VkSubpassDescription subpass = {};
    subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
    subpass.colorAttachmentCount = 1;
    subpass.pColorAttachments = &colorAttachmentRef;

    VkRenderPassCreateInfo renderPassInfo = {};
    renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
    renderPassInfo.attachmentCount = 1;
    renderPassInfo.pAttachments = &colorAttachment;
    renderPassInfo.subpassCount = 1;
    renderPassInfo.pSubpasses = &subpass;

    if (vkCreateRenderPass(device, &renderPassInfo, NULL, &renderPass) != VK_SUCCESS) {
        fprintf(stderr, "Failed to create render pass!\n");
        exit(EXIT_FAILURE);
    }

    printf("Vulkan render pass created.\n");
}

void createGraphicsPipeline() {
    char* vertexShaderCode = loadShaderCode("src/shader.vert");
    char* fragmentShaderCode = loadShaderCode("src/shader.frag");

    // Create vertex shader module
    VkShaderModuleCreateInfo vertexShaderCreateInfo = {};
    vertexShaderCreateInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
    vertexShaderCreateInfo.codeSize = strlen(vertexShaderCode);
    vertexShaderCreateInfo.pCode = (const uint32_t*)vertexShaderCode;
    if (vkCreateShaderModule(device, &vertexShaderCreateInfo, NULL, &vertexShaderModule) != VK_SUCCESS) {
        fprintf(stderr, "Failed to create vertex shader module!\n");
        exit(EXIT_FAILURE);
    }

    // Create fragment shader module
    VkShaderModuleCreateInfo fragmentShaderCreateInfo = {};
    fragmentShaderCreateInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
    fragmentShaderCreateInfo.codeSize = strlen(fragmentShaderCode);
    fragmentShaderCreateInfo.pCode = (const uint32_t*)fragmentShaderCode;
    if (vkCreateShaderModule(device, &fragmentShaderCreateInfo, NULL, &fragmentShaderModule) != VK_SUCCESS) {
        fprintf(stderr, "Failed to create fragment shader module!\n");
        exit(EXIT_FAILURE);
    }

    // Free the allocated memory for shader code
    free(vertexShaderCode);
    free(fragmentShaderCode);

    // VkGraphicsPipelineCreateInfo pipelineInfo = {};
    // pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
    // // pipelineInfo.stageCount = 2;
    // // pipelineInfo.pStages = shaderStages;
    // // pipelineInfo.pVertexInputState = &vertexInputInfo;
    // // pipelineInfo.pInputAssemblyState = &inputAssembly;
    // // pipelineInfo.pViewportState = &viewportState;
    // // pipelineInfo.pRasterizationState = &rasterizer;
    // // pipelineInfo.pMultisampleState = &multisampling;
    // // pipelineInfo.pDepthStencilState = &depthStencil;
    // // pipelineInfo.pColorBlendState = &colorBlending;
    // pipelineInfo.layout = pipelineLayout;
    // pipelineInfo.renderPass = renderPass;
    // pipelineInfo.subpass = 0;
    // pipelineInfo.basePipelineHandle = VK_NULL_HANDLE;

    // if (vkCreateGraphicsPipelines(device, VK_NULL_HANDLE, 1, &pipelineInfo, NULL, &graphicsPipeline) != VK_SUCCESS) {
    //     fprintf(stderr, "Failed to create graphics pipeline!\n");
    //     exit(EXIT_FAILURE);
    // }

}
