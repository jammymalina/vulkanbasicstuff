#include "utils.h" 

#include <stdlib.h>
#include <string.h>
#include <stddef.h>
#include "../logger/logger.h"

bool set_vulkan_device_feature_prop(VkPhysicalDeviceFeatures *features, const char *prop, VkBool32 val) {
	if (strcmp(prop, "robustBufferAccess") == 0) {
		features->robustBufferAccess = val;
		return true;
	}
	if (strcmp(prop, "fullDrawIndexUint32") == 0) {
		features->fullDrawIndexUint32 = val;
		return true;
	}
	if (strcmp(prop, "imageCubeArray") == 0) {
		features->imageCubeArray = val;
		return true;
	}
	if (strcmp(prop, "independentBlend") == 0) {
		features->independentBlend = val;
		return true;
	}
	if (strcmp(prop, "geometryShader") == 0) {
		features->geometryShader = val;
		return true;
	}
	if (strcmp(prop, "tessellationShader") == 0) {
		features->tessellationShader = val;
		return true;
	}
	if (strcmp(prop, "sampleRateShading") == 0) {
		features->sampleRateShading = val;
		return true;
	}
	if (strcmp(prop, "dualSrcBlend") == 0) {
		features->dualSrcBlend = val;
		return true;
	}
	if (strcmp(prop, "logicOp") == 0) {
		features->logicOp = val;
		return true;
	}
	if (strcmp(prop, "multiDrawIndirect") == 0) {
		features->multiDrawIndirect = val;
		return true;
	}
	if (strcmp(prop, "drawIndirectFirstInstance") == 0) {
		features->drawIndirectFirstInstance = val;
		return true;
	}
	if (strcmp(prop, "depthClamp") == 0) {
		features->depthClamp = val;
		return true;
	}
	if (strcmp(prop, "depthBiasClamp") == 0) {
		features->depthBiasClamp = val;
		return true;
	}
	if (strcmp(prop, "fillModeNonSolid") == 0) {
		features->fillModeNonSolid = val;
		return true;
	}
	if (strcmp(prop, "depthBounds") == 0) {
		features->depthBounds = val;
		return true;
	}
	if (strcmp(prop, "wideLines") == 0) {
		features->wideLines = val;
		return true;
	}
	if (strcmp(prop, "largePoints") == 0) {
		features->largePoints = val;
		return true;
	}
	if (strcmp(prop, "alphaToOne") == 0) {
		features->alphaToOne = val;
		return true;
	}
	if (strcmp(prop, "multiViewport") == 0) {
		features->multiViewport = val;
		return true;
	}
	if (strcmp(prop, "samplerAnisotropy") == 0) {
		features->samplerAnisotropy = val;
		return true;
	}
	if (strcmp(prop, "textureCompressionETC2") == 0) {
		features->textureCompressionETC2 = val;
		return true;
	}
	if (strcmp(prop, "textureCompressionASTC_LDR") == 0) {
		features->textureCompressionASTC_LDR = val;
		return true;
	}
	if (strcmp(prop, "textureCompressionBC") == 0) {
		features->textureCompressionBC = val;
		return true;
	}
	if (strcmp(prop, "occlusionQueryPrecise") == 0) {
		features->occlusionQueryPrecise = val;
		return true;
	}
	if (strcmp(prop, "pipelineStatisticsQuery") == 0) {
		features->pipelineStatisticsQuery = val;
		return true;
	}
	if (strcmp(prop, "vertexPipelineStoresAndAtomics") == 0) {
		features->vertexPipelineStoresAndAtomics = val;
		return true;
	}
	if (strcmp(prop, "fragmentStoresAndAtomics") == 0) {
		features->fragmentStoresAndAtomics = val;
		return true;
	}
	if (strcmp(prop, "shaderTessellationAndGeometryPointSize") == 0) {
		features->shaderTessellationAndGeometryPointSize = val;
		return true;
	}
	if (strcmp(prop, "shaderImageGatherExtended") == 0) {
		features->shaderImageGatherExtended = val;
		return true;
	}
	if (strcmp(prop, "shaderStorageImageExtendedFormats") == 0) {
		features->shaderStorageImageExtendedFormats = val;
		return true;
	}
	if (strcmp(prop, "shaderStorageImageMultisample") == 0) {
		features->shaderStorageImageMultisample = val;
		return true;
	}
	if (strcmp(prop, "shaderStorageImageReadWithoutFormat") == 0) {
		features->shaderStorageImageReadWithoutFormat = val;
		return true;
	}
	if (strcmp(prop, "shaderStorageImageWriteWithoutFormat") == 0) {
		features->shaderStorageImageWriteWithoutFormat = val;
		return true;
	}
	if (strcmp(prop, "shaderUniformBufferArrayDynamicIndexing") == 0) {
		features->shaderUniformBufferArrayDynamicIndexing = val;
		return true;
	}
	if (strcmp(prop, "shaderSampledImageArrayDynamicIndexing") == 0) {
		features->shaderSampledImageArrayDynamicIndexing = val;
		return true;
	}
	if (strcmp(prop, "shaderStorageBufferArrayDynamicIndexing") == 0) {
		features->shaderStorageBufferArrayDynamicIndexing = val;
		return true;
	}
	if (strcmp(prop, "shaderStorageImageArrayDynamicIndexing") == 0) {
		features->shaderStorageImageArrayDynamicIndexing = val;
		return true;
	}
	if (strcmp(prop, "shaderClipDistance") == 0) {
		features->shaderClipDistance = val;
		return true;
	}
	if (strcmp(prop, "shaderCullDistance") == 0) {
		features->shaderCullDistance = val;
		return true;
	}
	if (strcmp(prop, "shaderFloat64") == 0) {
		features->shaderFloat64 = val;
		return true;
	}
	if (strcmp(prop, "shaderInt64") == 0) {
		features->shaderInt64 = val;
		return true;
	}
	if (strcmp(prop, "shaderInt16") == 0) {
		features->shaderInt16 = val;
		return true;
	}
	if (strcmp(prop, "shaderResourceResidency") == 0) {
		features->shaderResourceResidency = val;
		return true;
	}
	if (strcmp(prop, "shaderResourceMinLod") == 0) {
		features->shaderResourceMinLod = val;
		return true;
	}
	if (strcmp(prop, "sparseBinding") == 0) {
		features->sparseBinding = val;
		return true;
	}
	if (strcmp(prop, "sparseResidencyBuffer") == 0) {
		features->sparseResidencyBuffer = val;
		return true;
	}
	if (strcmp(prop, "sparseResidencyImage2D") == 0) {
		features->sparseResidencyImage2D = val;
		return true;
	}
	if (strcmp(prop, "sparseResidencyImage3D") == 0) {
		features->sparseResidencyImage3D = val;
		return true;
	}
	if (strcmp(prop, "sparseResidency2Samples") == 0) {
		features->sparseResidency2Samples = val;
		return true;
	}
	if (strcmp(prop, "sparseResidency4Samples") == 0) {
		features->sparseResidency4Samples = val;
		return true;
	}
	if (strcmp(prop, "sparseResidency8Samples") == 0) {
		features->sparseResidency8Samples = val;
		return true;
	}
	if (strcmp(prop, "sparseResidency16Samples") == 0) {
		features->sparseResidency16Samples = val;
		return true;
	}
	if (strcmp(prop, "sparseResidencyAliased") == 0) {
		features->sparseResidencyAliased = val;
		return true;
	}
	if (strcmp(prop, "variableMultisampleRate") == 0) {
		features->variableMultisampleRate = val;
		return true;
	}
	if (strcmp(prop, "inheritedQueries") == 0) {
		features->inheritedQueries = val;
		return true;
	}
	return false;	
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

bool are_vulkan_extensions_supported(const VkExtensionProperties available_extensions[MAX_VULKAN_EXTENSIONS],
	uint32_t available_extensions_count, const char desired_extensions[MAX_VULKAN_EXTENSIONS][VK_MAX_EXTENSION_NAME_SIZE],
	uint32_t desired_extensions_count) 
{
	for (size_t i = 0; i < desired_extensions_count; i++) {
		if (!is_vulkan_extension_supported(available_extensions, available_extensions_count, desired_extensions[i])) {
			return false; 
		}
	}

	return true;	
}

bool is_device_supported(const vk_functions *vk, 
	VkPhysicalDevice device,
	const char desired_extensions[MAX_VULKAN_EXTENSIONS][VK_MAX_EXTENSION_NAME_SIZE], uint32_t desired_extensions_count,
	const VkPhysicalDeviceFeatures *desired_features) 
{	
	VkExtensionProperties available_extensions[MAX_VULKAN_EXTENSIONS];
	uint32_t available_extensions_count = 0;
	bool success = get_device_extensions(vk, device, available_extensions, &available_extensions_count);
	if (!success)
		return false;
	
	VkPhysicalDeviceFeatures available_features;
	get_device_features(vk, device, &available_features);

	return are_vulkan_extensions_supported(available_extensions, available_extensions_count, desired_extensions, 
		desired_extensions_count) && are_vulkan_device_features_supported(&available_features, desired_features);
}

bool get_available_extensions(const vk_functions *vk, 
	VkExtensionProperties *available_extensions, uint32_t *available_extensions_count) 
{
	*available_extensions_count = 0; 

	VkResult result = vk->EnumerateInstanceExtensionProperties(NULL, available_extensions_count, NULL);
	if (result != VK_SUCCESS || *available_extensions_count == 0) {
		error_log("Unable to get the number of Vulkan extensions");	
		return false;
	} 

	if (*available_extensions_count > MAX_VULKAN_EXTENSIONS) {
		error_log("Not enough space for extensions");
		return false;
	}
	
	result = vk->EnumerateInstanceExtensionProperties(NULL, available_extensions_count, 
		available_extensions);
	if (result != VK_SUCCESS || *available_extensions_count == 0) {
		error_log("Unable to enumerate Vulkan extensions");
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
		error_log("Unable to get the number of Vulkan devices");
		return false; 
	}

	if (*available_devices_count > MAX_VULKAN_DEVICES) {
		error_log("Not enough space for devices");
		return false;
	}
	
	result = vk->EnumeratePhysicalDevices(instance, available_devices_count, available_devices);
	if (result != VK_SUCCESS || *available_devices_count == 0) {
		error_log("Unable to enumerate Vulkan devices");
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
		error_log("Unable to get number of physical device extensions");
		return false;
	}

	if (*device_extensions_count > MAX_VULKAN_EXTENSIONS) {
		error_log("Not enough space for Vulkan extensions"); 
		return false;
	}	

	result = vk->EnumerateDeviceExtensionProperties
		(physical_device, NULL, device_extensions_count, device_extensions);
	if (result != VK_SUCCESS && *device_extensions_count == 0) {
		error_log("Unable to enumarete physical device extensions");
		return false; 
	}
	
	return true;
}

bool get_available_queue_props(const vk_functions *vk, VkPhysicalDevice physical_device,
	VkQueueFamilyProperties *queue_props, uint32_t *queue_props_count)
{
	*queue_props_count = 0;
	vk->GetPhysicalDeviceQueueFamilyProperties(physical_device, queue_props_count, NULL); 
	if (*queue_props_count == 0 || *queue_props_count > MAX_QUEUE_PROPS) 
		return false; 
	vk->GetPhysicalDeviceQueueFamilyProperties(physical_device, queue_props_count, queue_props);
	return *queue_props_count != 0;
}

bool get_available_present_modes(const vk_functions *vk, VkPhysicalDevice physical_device, 
	VkSurfaceKHR surface, VkPresentModeKHR *present_modes, uint32_t *present_modes_count) 
{
	*present_modes_count = 0;
	VkResult result = vk->GetPhysicalDeviceSurfacePresentModesKHR(physical_device, 
		surface, present_modes_count, NULL);
    if (result != VK_SUCCESS || *present_modes_count == 0) {
		error_log("Unable to get physical device surface present modes");
		return false;  
	}

	if (*present_modes_count > MAX_PRESENT_MODE_COUNT) {
		error_log("Not enough space for physical device surface present modes");
		return false;  
	}

	result = vk->GetPhysicalDeviceSurfacePresentModesKHR(physical_device, 
		surface, present_modes_count, present_modes);
	if (result != VK_SUCCESS || *present_modes_count == 0) {
		error_log("Unable to get physical device surface present modes");
		return false;  
	}

	return true;
}

bool get_surface_capabilities(const vk_functions *vk, VkPhysicalDevice physical_device, VkSurfaceKHR surface, 
	VkSurfaceCapabilitiesKHR *capabilities)
{
	VkResult result = vk->GetPhysicalDeviceSurfaceCapabilitiesKHR(physical_device, surface, capabilities);
	return result == VK_SUCCESS;
}

bool present_mode_from_string(VkPresentModeKHR *dest,  const char *m) {
	if (strcmp(m, "fifo") == 0) {
		*dest = VK_PRESENT_MODE_FIFO_KHR;
		return true;
	}
	if (strcmp(m, "immediate") == 0) {
		*dest = VK_PRESENT_MODE_IMMEDIATE_KHR;
		return true;
	}
	if (strcmp(m, "mailbox") == 0) {
		*dest = VK_PRESENT_MODE_MAILBOX_KHR;
		return true;
	}
	if (strcmp(m, "fifo_relaxed") == 0) {
		*dest = VK_PRESENT_MODE_FIFO_RELAXED_KHR;
		return true;
	}
	*dest = VK_PRESENT_MODE_FIFO_KHR;
	return false;
}

