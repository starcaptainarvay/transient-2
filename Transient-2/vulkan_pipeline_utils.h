#pragma once
#ifndef VULKAN_PIPELINE_UTILS_H
#define VULKAN_PIPELINE_UTILS_H

#include <vulkan/vulkan.h>
#include <optional>
#include <vector>

const std::vector<const char*> TRANSIENT_REQUIRED_DEVICE_EXTENSIONS = {
    VK_KHR_SWAPCHAIN_EXTENSION_NAME
};

void createInstance(VkInstance* pInstance);
void pickPhysicalDevice(VkInstance* pInstance, VkPhysicalDevice* pDevice, VkSurfaceKHR* surface);

struct QueueFamilyIndices {
    std::optional<uint32_t> graphicsFamily;
    std::optional<uint32_t> presentFamily;

    bool isComplete() {
        return graphicsFamily.has_value() && presentFamily.has_value();
    }
};

QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device, VkSurfaceKHR surface);
QueueFamilyIndices createLogicalDevice(VkDevice* pDevice, VkPhysicalDevice* pPhysicalDevice, VkSurfaceKHR* surface);

struct SwapChainSupportDetails {
    VkSurfaceCapabilitiesKHR capabilities;
    std::vector<VkSurfaceFormatKHR> formats;
    std::vector<VkPresentModeKHR> presentModes;
};

SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice device, VkSurfaceKHR surface);

#endif