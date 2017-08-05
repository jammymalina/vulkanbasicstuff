#include "string_utils.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

char* vulkan_version_to_string(uint32_t version) {
	char *version_string = (char*) malloc(256 * sizeof(char));
	uint32_t major_version = get_vulkan_version_major(version); 
	uint32_t minor_version = get_vulkan_version_minor(version); 
	uint32_t patch_version = get_vulkan_version_patch(version);
	snprintf(version_string, 256, "%d.%d.%d", major_version, minor_version, patch_version);
	return version_string;
}

void physical_device_features_log(VkPhysicalDeviceFeatures *device_features) {
	printf(
		"VkPhysicalDeviceFeatures: {\n" 	
		"  robustBufferAccess: %d\n"
		"  fullDrawIndexUint32: %d\n"
		"  imageCubeArray: %d\n"
		"  independentBlend: %d\n"
		"  geometryShader: %d\n"
		"  tessellationShader: %d\n"
		"  sampleRateShading: %d\n"
		"  dualSrcBlend: %d\n"
		"  logicOp: %d\n"
		"  multiDrawIndirect: %d\n"
		"  drawIndirectFirstInstance: %d\n"
		"  depthClamp: %d\n"
		"  depthBiasClamp: %d\n"
		"  fillModeNonSolid: %d\n"
		"  depthBounds: %d\n"
		"  wideLines: %d\n"
		"  largePoints: %d\n"
		"  alphaToOne: %d\n"
		"  multiViewport: %d\n"
		"  samplerAnisotropy: %d\n"
		"  textureCompressionETC2: %d\n"
		"  textureCompressionASTC_LDR: %d\n"
		"  textureCompressionBC: %d\n"
		"  occlusionQueryPrecise: %d\n"
		"  pipelineStatisticsQuery: %d\n"
		"  vertexPipelineStoresAndAtomics: %d\n"
		"  fragmentStoresAndAtomics: %d\n"
		"  shaderTessellationAndGeometryPointSize: %d\n"
		"  shaderImageGatherExtended: %d\n"
		"  shaderStorageImageExtendedFormats: %d\n"
		"  shaderStorageImageMultisample: %d\n"
		"  shaderStorageImageReadWithoutFormat: %d\n"
		"  shaderStorageImageWriteWithoutFormat: %d\n"
		"  shaderUniformBufferArrayDynamicIndexing: %d\n"
		"  shaderSampledImageArrayDynamicIndexing: %d\n"
		"  shaderStorageBufferArrayDynamicIndexing: %d\n"
		"  shaderStorageImageArrayDynamicIndexing: %d\n"
		"  shaderClipDistance: %d\n"
		"  shaderCullDistance: %d\n"
		"  shaderFloat64: %d\n"
		"  shaderInt64: %d\n"
		"  shaderInt16: %d\n"
		"  shaderResourceResidency: %d\n"
		"  shaderResourceMinLod: %d\n"
		"  sparseBinding: %d\n"
		"  sparseResidencyBuffer: %d\n"
		"  sparseResidencyImage2D: %d\n"
		"  sparseResidencyImage3D: %d\n"
		"  sparseResidency2Samples: %d\n"
		"  sparseResidency4Samples: %d\n"
		"  sparseResidency8Samples: %d\n"
		"  sparseResidency16Samples: %d\n"
		"  sparseResidencyAliased: %d\n"
		"  variableMultisampleRate: %d\n"
		"  inheritedQueries: %d\n"
		"}\n", 
		device_features->robustBufferAccess,
		device_features->fullDrawIndexUint32,
		device_features->imageCubeArray,
		device_features->independentBlend,
		device_features->geometryShader,
		device_features->tessellationShader,
		device_features->sampleRateShading,
		device_features->dualSrcBlend,
		device_features->logicOp,
		device_features->multiDrawIndirect,
		device_features->drawIndirectFirstInstance,
		device_features->depthClamp,
		device_features->depthBiasClamp,
		device_features->fillModeNonSolid,
		device_features->depthBounds,
		device_features->wideLines,
		device_features->largePoints,
		device_features->alphaToOne,
		device_features->multiViewport,
		device_features->samplerAnisotropy,
		device_features->textureCompressionETC2,
		device_features->textureCompressionASTC_LDR,
		device_features->textureCompressionBC,
		device_features->occlusionQueryPrecise,
		device_features->pipelineStatisticsQuery,
		device_features->vertexPipelineStoresAndAtomics,
		device_features->fragmentStoresAndAtomics,
		device_features->shaderTessellationAndGeometryPointSize,
		device_features->shaderImageGatherExtended,
		device_features->shaderStorageImageExtendedFormats,
		device_features->shaderStorageImageMultisample,
		device_features->shaderStorageImageReadWithoutFormat,
		device_features->shaderStorageImageWriteWithoutFormat,
		device_features->shaderUniformBufferArrayDynamicIndexing,
		device_features->shaderSampledImageArrayDynamicIndexing,
		device_features->shaderStorageBufferArrayDynamicIndexing,
		device_features->shaderStorageImageArrayDynamicIndexing,
		device_features->shaderClipDistance,
		device_features->shaderCullDistance,
		device_features->shaderFloat64,
		device_features->shaderInt64,
		device_features->shaderInt16,
		device_features->shaderResourceResidency,
		device_features->shaderResourceMinLod,
		device_features->sparseBinding,
		device_features->sparseResidencyBuffer,
		device_features->sparseResidencyImage2D,
		device_features->sparseResidencyImage3D,
		device_features->sparseResidency2Samples,
		device_features->sparseResidency4Samples,
		device_features->sparseResidency8Samples,
		device_features->sparseResidency16Samples,
		device_features->sparseResidencyAliased,
		device_features->variableMultisampleRate,
		device_features->inheritedQueries
	);
}

const char* physical_device_type_to_string(VkPhysicalDeviceType type) {
	switch (type) {
		case VK_PHYSICAL_DEVICE_TYPE_OTHER:
			return "Other";
		case VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU:
			return "Integrated GPU";
		case VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU:
			return "Discrete GPU"; 
		case VK_PHYSICAL_DEVICE_TYPE_VIRTUAL_GPU:
			return "Virtual GPU";
		case VK_PHYSICAL_DEVICE_TYPE_CPU:
			return "CPU";
		default: 
			return "Unknown";
	}
}

void physical_device_props_log(VkPhysicalDeviceProperties *device_props) {
	printf(
		"VkPhysicalDeviceProperties: {\n"
		"  apiVersion: %s\n"
		"  driverVersion: %s\n"
		"  vendorID: %d\n"
		"  deviceID: %d\n"
		"  deviceType: %s\n"
		"  deviceName: %s\n"	
		"}\n", 
		vulkan_version_to_string(device_props->apiVersion), 
		vulkan_version_to_string(device_props->driverVersion), 
		device_props->vendorID,
		device_props->deviceID,	
		physical_device_type_to_string(device_props->deviceType), 
		device_props->deviceName
	);
}

char* queue_flags_to_string(VkQueueFlags flags) {
	char *result = (char*) malloc(256 * sizeof(char));
	if (!result) {
		return NULL;
	}
	strcpy(result, "");
	if (flags & VK_QUEUE_GRAPHICS_BIT) {
		strcat(result, "graphics, "); 
	}	
	if (flags & VK_QUEUE_COMPUTE_BIT) {
		strcat(result, "compute, ");
	}
	if (flags & VK_QUEUE_TRANSFER_BIT) {
		strcat(result, "transfer, ");
	}
	if (flags & VK_QUEUE_SPARSE_BINDING_BIT) {
		strcat(result, "sparse management, ");
	}
	size_t len = strlen(result);
	result[len - 1] = '\0';
	result[len - 2] = '\0';
	return result;
}

void queue_props_log(const VkQueueFamilyProperties *props) {
	printf(
		"VkQueueFamilyProperties {\n"
		"  queueFlags: %s\n"
		"  queueCount: %d\n" 
		"  timestampValidBits: %d\n"
		"  minImageTransferGranularity: (width = %d, height = %d, depth = %d)\n"
		"}\n",
		queue_flags_to_string(props->queueFlags), 
		props->queueCount, 
		props->timestampValidBits, 
		props->minImageTransferGranularity.width, props->minImageTransferGranularity.height, 
		props->minImageTransferGranularity.depth
	);
}

