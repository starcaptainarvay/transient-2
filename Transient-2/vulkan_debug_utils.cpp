#include <vulkan/vulkan.h>

#include "vulkan_debug_utils.h"
#include <stdexcept>
#include <string>

VkPhysicalDeviceProperties getPhysicalDeviceProperties(VkPhysicalDevice* pDevice) {
    if (pDevice == VK_NULL_HANDLE)
        throw std::runtime_error("physical device is null!");
    VkPhysicalDeviceProperties props;
    vkGetPhysicalDeviceProperties(*pDevice, &props);
    return props;
}

VkPhysicalDeviceFeatures getPhysicalDeviceFeatures(VkPhysicalDevice* pDevice) {
    if (pDevice == VK_NULL_HANDLE)
        throw std::runtime_error("physical device is null!");
    VkPhysicalDeviceFeatures props;
    vkGetPhysicalDeviceFeatures(*pDevice, &props);
    return props;
}

void printPhysicalDeviceInfo(VkPhysicalDeviceProperties props) {
    if (!VERBOSE(2)) return;
    
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
    if (!VERBOSE(2)) return;

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

void printPhysicalDeviceInfo(VkPhysicalDevice* pDevice) {
    if (!VERBOSE(2)) return;

    VkPhysicalDeviceProperties props = getPhysicalDeviceProperties(pDevice);
    printPhysicalDeviceInfo(props);
}

std::string getPresentModeName(VkPresentModeKHR presentMode) {
    switch (presentMode) {
        case VK_PRESENT_MODE_IMMEDIATE_KHR:
            return "VK_PRESENT_MODE_IMMEDIATE_KHR";
        case VK_PRESENT_MODE_MAILBOX_KHR:
            return "VK_PRESENT_MODE_MAILBOX_KHR";
        case VK_PRESENT_MODE_FIFO_KHR:
            return "VK_PRESENT_MODE_FIFO_KHR";
        case VK_PRESENT_MODE_FIFO_RELAXED_KHR:
            return "VK_PRESENT_MODE_FIFO_RELAXED_KHR";
        case VK_PRESENT_MODE_SHARED_DEMAND_REFRESH_KHR:
            return "VK_PRESENT_MODE_SHARED_DEMAND_REFRESH_KHR";
        case VK_PRESENT_MODE_SHARED_CONTINUOUS_REFRESH_KHR:
            return "VK_PRESENT_MODE_SHARED_CONTINUOUS_REFRESH_KHR";
        case VK_PRESENT_MODE_MAX_ENUM_KHR:
            return "VK_PRESENT_MODE_MAX_ENUM_KHR";
        default:
            return "Unknown Present Mode";
    }
}

void printSwapChainSupportDetails(SwapChainSupportDetails details) {
    if (!VERBOSE(DEBUG_VERBOSE)) return;

    printf("SwapChainSupportDetails: {\n");
    printf("\tcapabilities.minImageCount: %d,\n", details.capabilities.minImageCount);
    printf("\tcapabilities.maxImageCount: %d,\n", details.capabilities.maxImageCount);

    printf("\tsurfaceFormats: {\n");
    for (const auto& format : details.formats) {
        printf("\t\t{ format: %d, colorSpace: %d },\n", format.format, format.colorSpace);
    }
    printf("\t},\n");

    printf("\tpresentModes: [\n");
    for (const auto& presentMode : details.presentModes) {
        printf("\t\t%s\n", getPresentModeName(presentMode).c_str());
    }
    printf("\t]\n}\n");
}