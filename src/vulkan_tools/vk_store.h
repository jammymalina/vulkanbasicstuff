#ifndef VULKAN_STORE_H
#define VULKAN_STORE_H 

#include <vulkan/vulkan.h>
#include <stdint.h>
#include <stdbool.h>
#include "../vulkan_functions/functions.h"

#define MAX_VULKAN_EXTENSIONS 256

typedef struct {
	uint32_t extensions_count; 
	VkExtensionProperties extensions[MAX_VULKAN_EXTENSIONS];	
} vk_store; 

bool load_extensions(const vk_functions *vk, vk_store *store);

#endif // VULKAN_STORE_H

