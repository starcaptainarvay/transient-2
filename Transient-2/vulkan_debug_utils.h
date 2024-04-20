#pragma once
#ifndef VULKAN_DEBUG_UTILS_H
#define VULKAN_DEBUG_UTILS_H

//Debug tiers
#define NO_DEBUG 0
#define PRINT_STEPS 1
#define DEBUG_VERBOSE 2

#define TRANSIENT_DEBUG_LEVEL DEBUG_VERBOSE
/*
* DEBUG_LEVEL:
*	= 0 -> no prints other than errors
*	= 1 -> basic one-time application state updates in terminal
*	= 2 -> most verbose. print hardware details as it's fetched
*/
#define VERBOSE(n) (TRANSIENT_DEBUG_LEVEL >= n)

#include <vulkan/vulkan.h>
#include "vulkan_pipeline_utils.h"

VkPhysicalDeviceProperties getPhysicalDeviceProperties(VkPhysicalDevice* pDevice);
VkPhysicalDeviceFeatures getPhysicalDeviceFeatures(VkPhysicalDevice* pDevice);

void printPhysicalDeviceInfo(VkPhysicalDevice* pDevice);
void printPhysicalDeviceInfo(VkPhysicalDeviceProperties props);
void printPhysicalDeviceFeatures(VkPhysicalDeviceProperties props, VkPhysicalDeviceFeatures features);

void printSwapChainSupportDetails(SwapChainSupportDetails details);

#endif