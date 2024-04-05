#ifndef VULKAN_PIPELINE_UTILS_H
#define VULKAN_PIPELINE_UTILS_H

#include <vulkan/vulkan.h>
#include <optional>

void createInstance(VkInstance *pInstance);
void pickPhysicalDevice(VkInstance *pInstance, VkPhysicalDevice *pDevice);

struct QueueFamilyIndices {
    std::optional<uint32_t> graphicsFamily;
};

QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device);

#endif