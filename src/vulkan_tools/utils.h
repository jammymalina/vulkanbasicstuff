#ifndef VULKAN_UTILS_H
#define VULKAN_UTILS_H

#include <vulkan/vulkan.h>
#include <stdint.h>
#include <stdbool.h>
#include "../vulkan_functions/functions.h"
#include "vulkan_limits.h" 

static inline uint32_t make_vulkan_version(uint32_t major, uint32_t minor, uint32_t patch) {
	return (major << 22) | (minor << 12) | patch;	
}

static inline void set_vulkan_device_features(VkPhysicalDeviceFeatures *features, VkBool32 val) {
	features->robustBufferAccess                      = val;
	features->fullDrawIndexUint32                     = val;
	features->imageCubeArray                          = val;
	features->independentBlend                        = val;
	features->geometryShader                          = val;
	features->tessellationShader                      = val;
	features->sampleRateShading                       = val;
	features->dualSrcBlend                            = val;
	features->logicOp                                 = val;
	features->multiDrawIndirect                       = val;
	features->drawIndirectFirstInstance               = val;
	features->depthClamp                              = val;
	features->depthBiasClamp                          = val;
	features->fillModeNonSolid                        = val;
	features->depthBounds                             = val;
	features->wideLines                               = val;
	features->largePoints                             = val;
	features->alphaToOne                              = val;
	features->multiViewport                           = val;
	features->samplerAnisotropy                       = val;
	features->textureCompressionETC2                  = val;
	features->textureCompressionASTC_LDR              = val;
	features->textureCompressionBC                    = val;
	features->occlusionQueryPrecise                   = val;
	features->pipelineStatisticsQuery                 = val;
	features->vertexPipelineStoresAndAtomics          = val;
	features->fragmentStoresAndAtomics                = val;
	features->shaderTessellationAndGeometryPointSize  = val;
	features->shaderImageGatherExtended               = val;
	features->shaderStorageImageExtendedFormats       = val;
	features->shaderStorageImageMultisample           = val;
	features->shaderStorageImageReadWithoutFormat     = val;
	features->shaderStorageImageWriteWithoutFormat    = val;
	features->shaderUniformBufferArrayDynamicIndexing = val;
	features->shaderSampledImageArrayDynamicIndexing  = val;
	features->shaderStorageBufferArrayDynamicIndexing = val;
	features->shaderStorageImageArrayDynamicIndexing  = val;
	features->shaderClipDistance                      = val;
	features->shaderCullDistance                      = val;
	features->shaderFloat64                           = val;
	features->shaderInt64                             = val;
	features->shaderInt16                             = val;
	features->shaderResourceResidency                 = val;
	features->shaderResourceMinLod                    = val;
	features->sparseBinding                           = val;
	features->sparseResidencyBuffer                   = val;
	features->sparseResidencyImage2D                  = val;
	features->sparseResidencyImage3D                  = val;
	features->sparseResidency2Samples                 = val;
	features->sparseResidency4Samples                 = val;
	features->sparseResidency8Samples                 = val;
	features->sparseResidency16Samples                = val;
	features->sparseResidencyAliased                  = val;
	features->variableMultisampleRate                 = val;
	features->inheritedQueries                        = val;
}

bool set_vulkan_device_feature_prop(VkPhysicalDeviceFeatures *features, const char *prop, VkBool32 val);

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

bool get_available_queue_props(const vk_functions *vk, VkPhysicalDevice physical_device,
	   	VkQueueFamilyProperties *queue_props, uint32_t *queue_props_count);

#endif // VULKAN_UTILS_H


