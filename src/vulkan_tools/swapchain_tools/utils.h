#ifndef SWAPCHAIN_UTILS_H
#define SWAPCHAIN_UTILS_H

#include <vulkan/vulkan.h>
#include <stdint.h>
#include <stddef.h>
#include "../../math/basic.h"

static inline bool set_swapchain_size(VkExtent2D *dest, 
    const VkSurfaceCapabilitiesKHR *capabilities, uint32_t width, uint32_t height) 
{
    if (capabilities->currentExtent.width == 0xFFFFFFFF) {
        dest->width  = clamp(width, capabilities->minImageExtent.width, capabilities->maxImageExtent.width);
        dest->height = clamp(height, capabilities->minImageExtent.height, capabilities->maxImageExtent.height);
        return true;
    } 
    *dest = capabilities->currentExtent;
    return false; 
}

static inline bool set_swapchain_scenario(VkImageUsageFlags *dest, const VkSurfaceCapabilitiesKHR *capabilities, 
    VkImageUsageFlags desired_usage) 
{
    *dest = desired_usage & capabilities->supportedUsageFlags;
    return *dest == desired_usage;
}

static inline bool set_swapchain_transformation(VkSurfaceTransformFlagBitsKHR *dest, 
    const VkSurfaceCapabilitiesKHR *capabilities, VkSurfaceTransformFlagBitsKHR desired_transform) 
{
    if (capabilities->supportedTransforms & desired_transform) {
        *dest = desired_transform;
        return true;
    } 

    *dest = capabilities->currentTransform;
    return false;
} 

static inline bool set_swapchain_format(VkSurfaceFormatKHR *dest, 
    const vk_functions *vk, VkPhysicalDevice physical_device, VkSurfaceKHR surface, 
    VkSurfaceFormatKHR *available_formats, uint32_t available_formats_count, 
    VkSurfaceFormatKHR desired_surface_format) 
{
    if (available_formats_count == 0) {
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
            *dest = desired_surface_format;
            return false;
        }
    }
    *dest = available_formats[0];
    return false; 
}

#endif // SWAPCHAIN_UTILS_H
