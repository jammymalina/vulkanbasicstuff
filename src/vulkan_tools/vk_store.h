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

	VkInstance instance;
	VkPhysicalDevice physical_device;
	VkDevice device;
	
	queue_balancer qb;	

	application_config application_info;
} vk_store; 

void init_vulkan_store(vk_store *store);
void destroy_vulkan_store(vk_store *store);
bool init_store_from_config(const vk_functions *vk, vk_store *store, const char *config_filename);
bool add_physical_device(const vk_functions *vk, vk_store *store);
bool add_queue_balancer(const vk_functions *vk, vk_store *store);
bool add_device(const vk_functions *vk, vk_store *store);

#endif // VULKAN_STORE_H

