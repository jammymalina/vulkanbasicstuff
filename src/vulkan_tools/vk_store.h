#ifndef VULKAN_STORE_H
#define VULKAN_STORE_H 

#include <vulkan/vulkan.h>
#include <stdint.h>
#include <stdbool.h>
#include "../vulkan_functions/functions.h"

#define MAX_VULKAN_EXTENSIONS 256
#define STORE_FIELD_SIZE 512

typedef struct {
	struct {
		uint32_t number; 
		long semanticForm[3];
	} apiversion;

	uint32_t extensions_count; 
	VkExtensionProperties extensions[MAX_VULKAN_EXTENSIONS];

	VkInstance instance;	

	struct {
		char title[STORE_FIELD_SIZE]; 
		char name[STORE_FIELD_SIZE]; 
		long version[3];
		struct {
			char name[STORE_FIELD_SIZE]; 
			long version[3];
		} engine;
	} application_info;
} vk_store; 

bool load_extensions(const vk_functions *vk, vk_store *store);
bool create_instance(const vk_functions *vk, vk_store *store, const char *config_filename); 

#endif // VULKAN_STORE_H

