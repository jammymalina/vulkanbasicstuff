#ifndef VULKAN_UTILS_H
#define VULKAN_UTILS_H

#include <vulkan/vulkan.h>
#include <stdint.h>
#include "vk_store.h" 

bool is_vulkan_extension_supported(const VkExtensionProperties available_extensions[MAX_VULKAN_EXTENSIONS],
	   	uint32_t extensions_count, const char *extension);

#endif 


