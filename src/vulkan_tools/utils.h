#ifndef VULKAN_UTILS_H
#define VULKAN_UTILS_H

#include <vulkan/vulkan.h>
#include <stdint.h>
#include <stdbool.h>
#include "vulkan_limits.h" 

bool is_vulkan_extension_supported(const VkExtensionProperties available_extensions[],
	   	uint32_t extensions_count, const char *extension);
uint32_t make_vulkan_version(uint32_t major, uint32_t minor, uint32_t patch);

#endif 


