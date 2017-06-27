#include "utils.h" 

#include <string.h>

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

uint32_t make_vulkan_version(uint32_t major, uint32_t minor, uint32_t patch) {
	return (((major) << 22) | ((minor) << 12) | (patch));	
}

