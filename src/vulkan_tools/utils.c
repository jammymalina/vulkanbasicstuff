#include "utils.h" 

#include <stddef.h>
#include <string.h>
#include "../logger/logger.h"

uint32_t make_vulkan_version(uint32_t major, uint32_t minor, uint32_t patch) {
	return (((major) << 22) | ((minor) << 12) | (patch));	
}

bool is_vulkan_extension_supported(const VkExtensionProperties available_extensions[MAX_VULKAN_EXTENSIONS],
	   	uint32_t extensions_count, const char *extension)
{
	for (size_t i = 0; i < extensions_count; i++) {
		if (strcmp(available_extensions[i].extensionName, extension) == 0) {
			return true;
		}		
	}
	return false;
}

bool get_available_extensions(const vk_functions *vk, 
	VkExtensionProperties *available_extensions, uint32_t *available_extensions_count) 
{
	*available_extensions_count = 0; 

	VkResult result = vk->EnumerateInstanceExtensionProperties(NULL, available_extensions_count, NULL);
	if (result != VK_SUCCESS || *available_extensions_count == 0) {
		error_log("Could not get the number of Vulkan extensions.");	
		return false;
	} 

	if (*available_extensions_count > MAX_VULKAN_EXTENSIONS) {
		error_log("Not enough space for extensions.");
		return false;
	}
	
	result = vk->EnumerateInstanceExtensionProperties(NULL, available_extensions_count, 
		available_extensions);
	if (result != VK_SUCCESS || *available_extensions_count == 0) {
		error_log("Could not enumerate Vulkan extensions.");
		return false;
	}

	return true;
}

bool get_available_devices(const vk_functions *vk, VkInstance instance,
	VkPhysicalDevice *available_devices, uint32_t *available_devices_count) 
{
	*available_devices_count = 0; 

	VkResult result = vk->EnumeratePhysicalDevices(instance, available_devices_count, NULL); 
	if (result != VK_SUCCESS || *available_devices_count == 0) {
		error_log("Could not get the number of Vulkan devices.");
	}

	if (*available_devices_count > MAX_VULKAN_DEVICES) {
		error_log("Not enough space for devices.");
		return false;
	}
	
	result = vk->EnumeratePhysicalDevices(instance, available_devices_count, available_devices);
	if (result != VK_SUCCESS || *available_devices_count == 0) {
		error_log("Could not enumerate Vulkan devices.");
		return false;
	}

	return true;
}

bool get_device_extensions(const vk_functions *vk, 
		VkPhysicalDevice physical_device, VkExtensionProperties *device_extensions,
		uint32_t *device_extensions_count)

{
	VkResult result = vk->EnumerateDeviceExtensionProperties
		(physical_device, NULL, device_extensions_count, NULL);
	if (result != VK_SUCCESS || *device_extensions_count == 0) {
		error_log("Unable get number of physical device extensions.");
		return false;
	}

	if (*device_extensions_count > MAX_VULKAN_EXTENSIONS) {
		error_log("Not enough space for Vulkan extensions."); 
		return false;
	}	

	result = vk->EnumerateDeviceExtensionProperties
		(physical_device, NULL, device_extensions_count, device_extensions);
	if (result != VK_SUCCESS && *device_extensions_count == 0) {
		error_log("Unable to enumarete physical device extensions.");
		return false; 
	}
	
	return true;
}

void get_device_features_and_props(const vk_functions *vk, VkPhysicalDevice physical_device, 
		VkPhysicalDeviceFeatures *device_features, VkPhysicalDeviceProperties *device_props) 
{
	vk->GetPhysicalDeviceFeatures(physical_device, device_features);
	vk->GetPhysicalDeviceProperties(physical_device, device_props);
}

void physical_device_features_log(VkPhysicalDeviceFeatures *device_features) {
}

void physical_device_props_log(VkPhysicalDeviceProperties *device_props) {
}

