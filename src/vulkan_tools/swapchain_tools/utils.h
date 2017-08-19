#ifndef SWAPCHAIN_UTILS_H
#define SWAPCHAIN_UTILS_H

#include <vulkan/vulkan.h>
#include <stdint.h>
#include <stddef.h>
#include "../../logger/logger.h"
#include "../../math/basic.h"

static inline bool set_swapchain_size(VkExtent2D *dest, 
    const VkSurfaceCapabilitiesKHR *capabilities, uint32_t width, uint32_t height) 
{
    if (capabilities->currentExtent.width == 0xFFFFFFFF) {
        dest->width  = clamp(width, capabilities->minImageExtent.width, capabilities->maxImageExtent.width);
        dest->height = clamp(height, capabilities->minImageExtent.height, capabilities->maxImageExtent.height);
        return true;
    } 
    error_log("Unable to set desired swapchain image size, using the default");
    *dest = capabilities->currentExtent;
    return false; 
}

static inline bool set_swapchain_scenario(VkImageUsageFlags *dest, const VkSurfaceCapabilitiesKHR *capabilities, 
    VkImageUsageFlags desired_usage) 
{
    *dest = desired_usage & capabilities->supportedUsageFlags;
    bool success = *dest == desired_usage;
    if (!success) {
        error_log("Unable to set desired image usage");
    }
    return success;
}

static inline bool set_swapchain_transformation(VkSurfaceTransformFlagBitsKHR *dest, 
    const VkSurfaceCapabilitiesKHR *capabilities, VkSurfaceTransformFlagBitsKHR desired_transform) 
{
    if (capabilities->supportedTransforms & desired_transform) {
        *dest = desired_transform;
        return true;
    } 

    error_log("Unable to set desired image transform, using the default");
    *dest = capabilities->currentTransform;
    return false;
} 

static inline bool set_swapchain_format(VkSurfaceFormatKHR *dest, 
    const vk_functions *vk, VkPhysicalDevice physical_device, VkSurfaceKHR surface, 
    VkSurfaceFormatKHR *available_formats, uint32_t available_formats_count, 
    VkSurfaceFormatKHR desired_surface_format) 
{
    if (available_formats_count == 0) {
        error_log("No available formats");
        return false;
    }

    if (available_formats_count == 1 && available_formats[0].format == VK_FORMAT_UNDEFINED) {
        *dest = desired_surface_format;
        return true;
    }

    for (size_t i = 0; i < available_formats_count; i++) {
        if (available_formats[i].format == desired_surface_format.format &&
            available_formats[i].colorSpace == desired_surface_format.colorSpace) 
        {
            *dest = desired_surface_format;
            return true;
        }
    }

    for (size_t i = 0; i < available_formats_count; i++) {
        if (available_formats[i].format == desired_surface_format.format)
        {
            error_log("The desired combination of surface format is unsupported, "
                "selected other color space");
            *dest = desired_surface_format;
            return false;
        }
    }
    error_log("The desired combination of surface format is unsupported, "
        "selected the first one available");
    *dest = available_formats[0];
    return false; 
}

#endif // SWAPCHAIN_UTILS_H
