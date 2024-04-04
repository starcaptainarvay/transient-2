#ifndef VULKAN_DEBUG_UTILS_H
#define VULKAN_DEBUG_UTILS_H

#include <vulkan/vulkan.h>

VkPhysicalDeviceProperties getPhysicalDeviceProperties(VkPhysicalDevice *pDevice);
void printPhysicalDeviceInfo(VkPhysicalDevice *pDevice);
void printPhysicalDeviceInfo(VkPhysicalDeviceProperties props);

#endif