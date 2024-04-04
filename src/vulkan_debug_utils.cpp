#include <vulkan/vulkan.h>

#include "vulkan_debug_utils.h"
#include <stdexcept>

VkPhysicalDeviceProperties getPhysicalDeviceProperties(VkPhysicalDevice *pDevice) {
    if (pDevice == VK_NULL_HANDLE)
        throw std::runtime_error("physical device is null!");
    VkPhysicalDeviceProperties props;
    vkGetPhysicalDeviceProperties(*pDevice, &props);
    return props;
}

VkPhysicalDeviceFeatures getPhysicalDeviceFeatures(VkPhysicalDevice *pDevice) {
    if (pDevice == VK_NULL_HANDLE)
        throw std::runtime_error("physical device is null!");
    VkPhysicalDeviceFeatures props;
    vkGetPhysicalDeviceFeatures(*pDevice, &props);
    return props;
}

void printPhysicalDeviceInfo(VkPhysicalDeviceProperties props) {
    printf("Device ID: %d {\n", props.deviceID);
    printf("\tName: %s\n", props.deviceName);
    // printf("\tType: %s\n", props.deviceType);
    const char* fType = "\tType: %s\n";

    switch (props.deviceType) {
        case VK_PHYSICAL_DEVICE_TYPE_OTHER:
            printf(fType, "OTHER");
            break;
        case VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU:
            printf(fType, "INTEGRATED_GPU");
            break;
        case VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU:
            printf(fType, "DISCRETE_GPU");
            break;
        case VK_PHYSICAL_DEVICE_TYPE_VIRTUAL_GPU:
            printf(fType, "VIRTUAL_GPU");
            break;
        case VK_PHYSICAL_DEVICE_TYPE_CPU:
            printf(fType, "CPU");
            break;
        default:
            break;
    }

    printf("\tDriver version: %d\n", props.driverVersion);
    printf("\tVendor ID: %d\n", props.vendorID);
    printf("\tAPI version: %d\n", props.apiVersion);
    printf("}\n");
}

void printPhysicalDeviceFeatures(VkPhysicalDeviceProperties props, VkPhysicalDeviceFeatures features) {
    printf("Device ID: %d {\n", props.deviceID);
    printf("\tName: %s\n", props.deviceName);
    // printf("\tType: %s\n", props.deviceType);
    const char* fType = "\tType: %s\n";

    switch (props.deviceType) {
        case VK_PHYSICAL_DEVICE_TYPE_OTHER:
            printf(fType, "OTHER");
            break;
        case VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU:
            printf(fType, "INTEGRATED_GPU");
            break;
        case VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU:
            printf(fType, "DISCRETE_GPU");
            break;
        case VK_PHYSICAL_DEVICE_TYPE_VIRTUAL_GPU:
            printf(fType, "VIRTUAL_GPU");
            break;
        case VK_PHYSICAL_DEVICE_TYPE_CPU:
            printf(fType, "CPU");
            break;
        default:
            break;
    }

    printf("\tAlpha to one: %d\n", features.alphaToOne);
    printf("}\n");
}

void printPhysicalDeviceInfo(VkPhysicalDevice *pDevice) {
    VkPhysicalDeviceProperties props = getPhysicalDeviceProperties(pDevice);
    printPhysicalDeviceInfo(props);
}