#pragma once
#ifndef VULKAN_DEBUG_UTILS_H
#define VULKAN_DEBUG_UTILS_H

#include <vulkan/vulkan.h>

VkPhysicalDeviceProperties getPhysicalDeviceProperties(VkPhysicalDevice* pDevice);
VkPhysicalDeviceFeatures getPhysicalDeviceFeatures(VkPhysicalDevice* pDevice);

void printPhysicalDeviceInfo(VkPhysicalDevice* pDevice);
void printPhysicalDeviceInfo(VkPhysicalDeviceProperties props);
void printPhysicalDeviceFeatures(VkPhysicalDeviceProperties props, VkPhysicalDeviceFeatures features);

#endif