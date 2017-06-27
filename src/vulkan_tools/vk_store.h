#ifndef VULKAN_STORE_H
#define VULKAN_STORE_H 

#include <vulkan/vulkan.h>
#include <stdint.h>
#include <stdbool.h>
#include "../vulkan_functions/functions.h"
#include "vulkan_limits.h"

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

	uint32_t available_extensions_count; 
	VkExtensionProperties available_extensions[MAX_VULKAN_EXTENSIONS];

	uint32_t loaded_extensions_count;
	char loaded_extensions[VK_MAX_EXTENSION_NAME_SIZE * MAX_VULKAN_EXTENSIONS];

	VkInstance instance;	

	application_config application_info;
} vk_store; 

void init_default_application_config(application_config *app_info);
void copy_application_config(application_config *dest, const application_config *src);
void application_config_log(const application_config *app_info);

void init_default_vulkan_config(vulkan_config *vk_info); 
void copy_vulkan_config(vulkan_config *dest, const vulkan_config *src); 
void vulkan_config_log(const vulkan_config *vk_info);

bool load_extensions(const vk_functions *vk, vk_store *store);
bool create_instance(const vk_functions *vk, vk_store *store, const char *config_filename); 

#endif // VULKAN_STORE_H

