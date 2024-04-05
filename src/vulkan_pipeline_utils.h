#ifndef VULKAN_PIPELINE_UTILS_H
#define VULKAN_PIPELINE_UTILS_H

#include <vulkan/vulkan.h>
#include <optional>

void createInstance(VkInstance *pInstance);
void pickPhysicalDevice(VkInstance *pInstance, VkPhysicalDevice *pDevice);

struct QueueFamilyIndices {
    std::optional<uint32_t> graphicsFamily;
    std::optional<uint32_t> presentFamily;

    bool isComplete() {
        return graphicsFamily.has_value() && presentFamily.has_value();
    }
};

QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device, VkSurfaceKHR surface);
QueueFamilyIndices createLogicalDevice(VkDevice *pDevice, VkPhysicalDevice *pPhysicalDevice, VkSurfaceKHR *surface);

#endif