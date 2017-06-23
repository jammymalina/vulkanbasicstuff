#include "vk_store.h"

#include "../logger/logger.h"

bool load_extensions(const vk_functions *vk, vk_store *store) {
	store->extensions_count = 0; 
	VkResult result = VK_SUCCESS; 

	result = vk->EnumerateInstanceExtensionProperties(NULL, &store->extensions_count, NULL);
	if (result != VK_SUCCESS || store->extensions_count == 0) {
		error_log("Could not get the Vulkan extensions.");	
		return false;
	} else {
		debug_log("Found %d extensions.", store->extensions_count);
	}
	if (store->extensions_count > MAX_VULKAN_EXTENSIONS) {
		error_log("Not enough space for extensions");
		return false;
	}
	
	result = vk->EnumerateInstanceExtensionProperties(NULL, &store->extensions_count, &store->extensions[0]);
	if (result != VK_SUCCESS || store->extensions_count == 0) {
		error_log("Could not enumerate Vulkan extensions.");
		return false;
	} else {
		debug_log("Successfully enumerated %d extensions.", store->extensions_count);
	}

	return true;
}

