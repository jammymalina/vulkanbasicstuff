#ifndef VULKAN_UTILS_H
#define VULKAN_UTILS_H

#include <vulkan/vulkan.h>
#include <stdint.h>
#include <stdbool.h>
#include "../vulkan_functions/functions.h"
#include "vulkan_limits.h" 

uint32_t make_vulkan_version(uint32_t major, uint32_t minor, uint32_t patch);

bool is_vulkan_extension_supported(const VkExtensionProperties available_extensions[],
	   	uint32_t extensions_count, const char *extension);
bool get_available_extensions(const vk_functions *vk, 
	VkExtensionProperties *available_extensions, uint32_t *available_extensions_count); 

bool get_available_devices(const vk_functions *vk, VkInstance instance,
	VkPhysicalDevice *available_devices, uint32_t *available_devices_count);
bool get_device_extensions(const vk_functions *vk, 
		VkPhysicalDevice physical_device, VkExtensionProperties *device_extensions, 
		uint32_t *device_extensions_count);
void get_device_features_and_props(const vk_functions *vk, VkPhysicalDevice physical_device, 
		VkPhysicalDeviceFeatures *device_features, VkPhysicalDeviceProperties *device_props);

void physical_device_features_log(VkPhysicalDeviceFeatures *device_features); 
void physical_device_props_log(VkPhysicalDeviceProperties *device_props);

#endif // VULKAN_UTILS_H


