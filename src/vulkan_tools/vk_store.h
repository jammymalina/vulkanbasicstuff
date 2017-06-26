#ifndef VULKAN_STORE_H
#define VULKAN_STORE_H 

#include <vulkan/vulkan.h>
#include <stdint.h>
#include <stdbool.h>
#include "../vulkan_functions/functions.h"

#define MAX_VULKAN_EXTENSIONS 256
#define STORE_FIELD_SIZE 512

typedef struct {
	char title[STORE_FIELD_SIZE]; 
	char name[STORE_FIELD_SIZE]; 
	unsigned long version[3];
	struct {
		char name[STORE_FIELD_SIZE]; 
		unsigned long version[3];
	} engine;
} application_config;

typedef struct {
	uint32_t desired_extensions_count;
	char desired_extensions[MAX_VULKAN_EXTENSIONS][VK_MAX_EXTENSION_NAME_SIZE];
	unsigned long desired_version[3];
} vulkan_config;

typedef struct {
	uint32_t apiVersion;

	uint32_t extensions_count; 
	VkExtensionProperties extensions[MAX_VULKAN_EXTENSIONS];

	VkInstance instance;	

	application_config application_info;
} vk_store; 

void init_default_application_config(application_config *config);
void copy_application_config(application_config *dest, application_config *src);

bool load_extensions(const vk_functions *vk, vk_store *store);
bool create_instance(const vk_functions *vk, vk_store *store, const char *config_filename); 

#endif // VULKAN_STORE_H

