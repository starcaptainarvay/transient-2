#ifndef VULKAN_PIPELINE_UTILS_H
#define VULKAN_PIPELINE_UTILS_H

#include <vulkan/vulkan.h>

void createInstance(VkInstance *pInstance);
void pickPhysicalDevice(VkInstance *pInstance, VkPhysicalDevice *pDevice);

#endif