#ifndef VULKAN_CONFIG_H
#define VULKAN_CONFIG_H

#include <vulkan/vulkan.h>
#include <stdint.h>
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
	unsigned long desired_version[3];

	uint32_t desired_extensions_count;
	char desired_extensions[MAX_VULKAN_EXTENSIONS][VK_MAX_EXTENSION_NAME_SIZE];

	uint32_t desired_device_extensions_count;
	char desired_device_extensions[MAX_VULKAN_EXTENSIONS][VK_MAX_EXTENSION_NAME_SIZE];

	VkPhysicalDeviceFeatures desired_device_features;
} vulkan_config;

typedef struct {
	char name[MAX_QUEUE_NAME_SIZE];

	VkQueueFamilyProperties properties;

	uint32_t queue_family_index;
	uint32_t queue_count;

	float priorities[MAX_QUEUE_COUNT];
} queue_family_config;

void init_default_application_config(application_config *app_info);
void copy_application_config(application_config *dest, const application_config *src);
void application_config_log(const application_config *app_info);

void init_default_vulkan_config(vulkan_config *vk_info); 
void copy_vulkan_config(vulkan_config *dest, const vulkan_config *src); 
void vulkan_config_log(const vulkan_config *vk_info);

void init_default_queue_config(queue_family_config *queue_info);
void copy_queue_config(queue_family_config *dest, const queue_family_config *src); 
void queue_config_log(const queue_family_config *queue_info);

#endif // VULKAN_CONFIG_H

