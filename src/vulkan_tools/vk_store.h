#ifndef VULKAN_STORE_H
#define VULKAN_STORE_H 

#include <vulkan/vulkan.h>
#include <stdint.h>
#include <stdbool.h>
#include "../vulkan_functions/functions.h"
#include "vulkan_limits.h"
#include "parser.h"
#include "vk_config.h"
#include "queue_balancer.h"

typedef struct {
	uint32_t apiVersion;

	uint32_t loaded_extensions_count;
	char loaded_extensions[MAX_VULKAN_EXTENSIONS][VK_MAX_EXTENSION_NAME_SIZE];

	uint32_t loaded_device_extensions_count;
	char loaded_device_extensions[MAX_VULKAN_EXTENSIONS][VK_MAX_EXTENSION_NAME_SIZE];
	VkPhysicalDeviceFeatures device_features;

	VkInstance instance;
	VkPhysicalDevice physical_device;
	VkDevice device;
	
	queue_balancer qb;	

	application_config application_info;
} vk_store; 

void init_vulkan_store(vk_store *store);
void destroy_vulkan_store(const vk_functions *vk, vk_store *store);
bool init_store_from_config(vk_functions *vk, vk_store *store, const char *config_filename);

#endif // VULKAN_STORE_H
