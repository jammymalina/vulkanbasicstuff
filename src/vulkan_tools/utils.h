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

static inline bool are_vulkan_device_features_supported(const VkPhysicalDeviceFeatures *available_features, 
	const VkPhysicalDeviceFeatures *desired_features) 
{	
	return
		(!desired_features->robustBufferAccess ||
			(desired_features->robustBufferAccess == available_features->robustBufferAccess)) &&
		(!desired_features->fullDrawIndexUint32 ||
			(desired_features->fullDrawIndexUint32 == available_features->fullDrawIndexUint32)) &&
		(!desired_features->imageCubeArray ||
			(desired_features->imageCubeArray == available_features->imageCubeArray)) &&
		(!desired_features->independentBlend ||
			(desired_features->independentBlend == available_features->independentBlend)) &&
		(!desired_features->geometryShader ||
			(desired_features->geometryShader == available_features->geometryShader)) &&
		(!desired_features->tessellationShader ||
			(desired_features->tessellationShader == available_features->tessellationShader)) &&
		(!desired_features->sampleRateShading ||
			(desired_features->sampleRateShading == available_features->sampleRateShading)) &&
		(!desired_features->dualSrcBlend ||
			(desired_features->dualSrcBlend == available_features->dualSrcBlend)) &&
		(!desired_features->logicOp ||
			(desired_features->logicOp == available_features->logicOp)) &&
		(!desired_features->multiDrawIndirect ||
			(desired_features->multiDrawIndirect == available_features->multiDrawIndirect)) &&
		(!desired_features->drawIndirectFirstInstance ||
			(desired_features->drawIndirectFirstInstance == available_features->drawIndirectFirstInstance)) &&
		(!desired_features->depthClamp ||
			(desired_features->depthClamp == available_features->depthClamp)) &&
		(!desired_features->depthBiasClamp ||
			(desired_features->depthBiasClamp == available_features->depthBiasClamp)) &&
		(!desired_features->fillModeNonSolid ||
			(desired_features->fillModeNonSolid == available_features->fillModeNonSolid)) &&
		(!desired_features->depthBounds ||
			(desired_features->depthBounds == available_features->depthBounds)) &&
		(!desired_features->wideLines ||
			(desired_features->wideLines == available_features->wideLines)) &&
		(!desired_features->largePoints ||
			(desired_features->largePoints == available_features->largePoints)) &&
		(!desired_features->alphaToOne ||
			(desired_features->alphaToOne == available_features->alphaToOne)) &&
		(!desired_features->multiViewport ||
			(desired_features->multiViewport == available_features->multiViewport)) &&
		(!desired_features->samplerAnisotropy ||
			(desired_features->samplerAnisotropy == available_features->samplerAnisotropy)) &&
		(!desired_features->textureCompressionETC2 ||
			(desired_features->textureCompressionETC2 == available_features->textureCompressionETC2)) &&
		(!desired_features->textureCompressionASTC_LDR ||
			(desired_features->textureCompressionASTC_LDR == available_features->textureCompressionASTC_LDR)) &&
		(!desired_features->textureCompressionBC ||
			(desired_features->textureCompressionBC == available_features->textureCompressionBC)) &&
		(!desired_features->occlusionQueryPrecise ||
			(desired_features->occlusionQueryPrecise == available_features->occlusionQueryPrecise)) &&
		(!desired_features->pipelineStatisticsQuery ||
			(desired_features->pipelineStatisticsQuery == available_features->pipelineStatisticsQuery)) &&
		(!desired_features->vertexPipelineStoresAndAtomics ||
			(desired_features->vertexPipelineStoresAndAtomics == available_features->vertexPipelineStoresAndAtomics)) &&
		(!desired_features->fragmentStoresAndAtomics ||
			(desired_features->fragmentStoresAndAtomics == available_features->fragmentStoresAndAtomics)) &&
		(!desired_features->shaderTessellationAndGeometryPointSize ||
			(desired_features->shaderTessellationAndGeometryPointSize == available_features->shaderTessellationAndGeometryPointSize)) &&
		(!desired_features->shaderImageGatherExtended ||
			(desired_features->shaderImageGatherExtended == available_features->shaderImageGatherExtended)) &&
		(!desired_features->shaderStorageImageExtendedFormats ||
			(desired_features->shaderStorageImageExtendedFormats == available_features->shaderStorageImageExtendedFormats)) &&
		(!desired_features->shaderStorageImageMultisample ||
			(desired_features->shaderStorageImageMultisample == available_features->shaderStorageImageMultisample)) &&
		(!desired_features->shaderStorageImageReadWithoutFormat ||
			(desired_features->shaderStorageImageReadWithoutFormat == available_features->shaderStorageImageReadWithoutFormat)) &&
		(!desired_features->shaderStorageImageWriteWithoutFormat ||
			(desired_features->shaderStorageImageWriteWithoutFormat == available_features->shaderStorageImageWriteWithoutFormat)) &&
		(!desired_features->shaderUniformBufferArrayDynamicIndexing ||
			(desired_features->shaderUniformBufferArrayDynamicIndexing == available_features->shaderUniformBufferArrayDynamicIndexing)) &&
		(!desired_features->shaderSampledImageArrayDynamicIndexing ||
			(desired_features->shaderSampledImageArrayDynamicIndexing == available_features->shaderSampledImageArrayDynamicIndexing)) &&
		(!desired_features->shaderStorageBufferArrayDynamicIndexing ||
			(desired_features->shaderStorageBufferArrayDynamicIndexing == available_features->shaderStorageBufferArrayDynamicIndexing)) &&
		(!desired_features->shaderStorageImageArrayDynamicIndexing ||
			(desired_features->shaderStorageImageArrayDynamicIndexing == available_features->shaderStorageImageArrayDynamicIndexing)) &&
		(!desired_features->shaderClipDistance ||
			(desired_features->shaderClipDistance == available_features->shaderClipDistance)) &&
		(!desired_features->shaderCullDistance ||
			(desired_features->shaderCullDistance == available_features->shaderCullDistance)) &&
		(!desired_features->shaderFloat64 ||
			(desired_features->shaderFloat64 == available_features->shaderFloat64)) &&
		(!desired_features->shaderInt64 ||
			(desired_features->shaderInt64 == available_features->shaderInt64)) &&
		(!desired_features->shaderInt16 ||
			(desired_features->shaderInt16 == available_features->shaderInt16)) &&
		(!desired_features->shaderResourceResidency ||
			(desired_features->shaderResourceResidency == available_features->shaderResourceResidency)) &&
		(!desired_features->shaderResourceMinLod ||
			(desired_features->shaderResourceMinLod == available_features->shaderResourceMinLod)) &&
		(!desired_features->sparseBinding ||
			(desired_features->sparseBinding == available_features->sparseBinding)) &&
		(!desired_features->sparseResidencyBuffer ||
			(desired_features->sparseResidencyBuffer == available_features->sparseResidencyBuffer)) &&
		(!desired_features->sparseResidencyImage2D ||
			(desired_features->sparseResidencyImage2D == available_features->sparseResidencyImage2D)) &&
		(!desired_features->sparseResidencyImage3D ||
			(desired_features->sparseResidencyImage3D == available_features->sparseResidencyImage3D)) &&
		(!desired_features->sparseResidency2Samples ||
			(desired_features->sparseResidency2Samples == available_features->sparseResidency2Samples)) &&
		(!desired_features->sparseResidency4Samples ||
			(desired_features->sparseResidency4Samples == available_features->sparseResidency4Samples)) &&
		(!desired_features->sparseResidency8Samples ||
			(desired_features->sparseResidency8Samples == available_features->sparseResidency8Samples)) &&
		(!desired_features->sparseResidency16Samples ||
			(desired_features->sparseResidency16Samples == available_features->sparseResidency16Samples)) &&
		(!desired_features->sparseResidencyAliased ||
			(desired_features->sparseResidencyAliased == available_features->sparseResidencyAliased)) &&
		(!desired_features->variableMultisampleRate ||
			(desired_features->variableMultisampleRate == available_features->variableMultisampleRate)) &&
		(!desired_features->inheritedQueries ||
			(desired_features->inheritedQueries == available_features->inheritedQueries));
}

bool set_vulkan_device_feature_prop(VkPhysicalDeviceFeatures *features, const char *prop, VkBool32 val);

bool is_vulkan_extension_supported(const VkExtensionProperties available_extensions[],
	   	uint32_t extensions_count, const char *extension);
bool are_vulkan_extensions_supported(const VkExtensionProperties available_extensions[MAX_VULKAN_EXTENSIONS],
	uint32_t available_extensions_count, const char desired_extensions[MAX_VULKAN_EXTENSIONS][VK_MAX_EXTENSION_NAME_SIZE],
	uint32_t desired_extensions_count); 
bool is_device_supported(const vk_functions *vk, 
	VkPhysicalDevice device, 
	const char desired_extensions[MAX_VULKAN_EXTENSIONS][VK_MAX_EXTENSION_NAME_SIZE], uint32_t desired_extensions_count,
	const VkPhysicalDeviceFeatures *desired_features);
bool get_available_extensions(const vk_functions *vk, 
	VkExtensionProperties *available_extensions, uint32_t *available_extensions_count); 

// device

bool get_available_devices(const vk_functions *vk, VkInstance instance,
	VkPhysicalDevice *available_devices, uint32_t *available_devices_count);
bool get_device_extensions(const vk_functions *vk, 
	VkPhysicalDevice physical_device, VkExtensionProperties *device_extensions, 
	uint32_t *device_extensions_count);

static inline void get_device_features(const vk_functions *vk, VkPhysicalDevice physical_device, 
	VkPhysicalDeviceFeatures *device_features)
{
	vk->GetPhysicalDeviceFeatures(physical_device, device_features);
}

static inline void get_device_props(const vk_functions *vk, VkPhysicalDevice physical_device, 
	VkPhysicalDeviceProperties *device_props)
{
	vk->GetPhysicalDeviceProperties(physical_device, device_props);
}

static inline void get_device_features_and_props(const vk_functions *vk, VkPhysicalDevice physical_device, 
	VkPhysicalDeviceFeatures *device_features, VkPhysicalDeviceProperties *device_props) 
{
	get_device_features(vk, physical_device, device_features);
	get_device_props(vk, physical_device, device_props);
}

bool get_available_queue_props(const vk_functions *vk, VkPhysicalDevice physical_device,
	VkQueueFamilyProperties *queue_props, uint32_t *queue_props_count);

// surface

bool get_available_present_modes(const vk_functions *vk, VkPhysicalDevice physical_device, 
	VkSurfaceKHR surface, VkPresentModeKHR *present_modes, uint32_t *present_modes_count);
bool get_surface_capabilities(const vk_functions *vk, VkPhysicalDevice physical_device, VkSurfaceKHR surface, 
	VkSurfaceCapabilitiesKHR *capabilities);
bool get_available_surface_formats(const vk_functions *vk, VkPhysicalDevice physical_device, VkSurfaceKHR surface,
	VkSurfaceFormatKHR *formats, uint32_t *formats_count);
bool get_swapchain_image_handles(const vk_functions *vk, VkDevice device, VkSwapchainKHR swapchain,
	VkImage *swapchain_images, uint32_t *swapchain_images_count);
bool get_swapchain_image_views(VkImageView *swapchain_image_views, uint32_t *swapchain_image_view_count,
	const vk_functions *vk, VkDevice device, VkFormat swapchain_image_format, VkImage *swapchain_images,
	uint32_t swapchain_image_count, VkComponentMapping component_mapping, VkImageSubresourceRange subresource_range); 
bool present_mode_from_string(VkPresentModeKHR *dest, const char *m);


#endif // VULKAN_UTILS_H


