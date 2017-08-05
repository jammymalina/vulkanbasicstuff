#include "vk_config.h"

#include <string.h>
#include <stdio.h>
#include "utils.h"
#include "string_utils.h"
#include "vulkan_limits.h"

void init_default_application_config(application_config *app_info) {
	strcpy(app_info->title, "Default title"); 
	strcpy(app_info->name, "Default name");
	app_info->version[0] = 1; 
	app_info->version[1] = app_info->version[2] = 0; 
	strcpy(app_info->engine.name, "Default engine name");
	app_info->engine.version[0] = 1;
    app_info->engine.version[1] = app_info->engine.version[2] = 0; 
}

void copy_application_config(application_config *dest, const application_config *src) {
	strcpy(dest->title,src->title); 
	strcpy(dest->name, src->name);
	dest->version[0] = src->version[0]; 
	dest->version[1] = src->version[1]; 
	dest->version[2] = src->version[2]; 
	strcpy(dest->engine.name, src->engine.name);
	dest->engine.version[0] = src->engine.version[0];
    dest->engine.version[1] = src->engine.version[1];
	dest->engine.version[2] = src->engine.version[2]; 
}

void application_config_log(const application_config *app_info) { 
	printf( 
		"application: {\n"
		"  name: %s,\n"
		"  title: %s,\n"
		"  version: %lu.%lu.%lu,\n"
		"  engine: {\n"
		"    name: %s,\n"
		"    version: %lu.%lu.%lu\n"
		"  },\n"
		"}\n",
		app_info->name, app_info->title, app_info->version[0], app_info->version[1], app_info->version[2], 
		app_info->engine.name, 
		app_info->engine.version[0], app_info->engine.version[1], app_info->engine.version[2]
	);
}

void init_default_vulkan_config(vulkan_config *vk_info) {
	vk_info->desired_version[0] = 1;
	vk_info->desired_version[1] = 0;
	vk_info->desired_version[2] = 0;
	vk_info->desired_extensions_count = 0; 
	set_vulkan_device_features(&vk_info->desired_device_features, VK_FALSE);
}

void copy_vulkan_config(vulkan_config *dest, const vulkan_config *src) {
	dest->desired_version[0] = src->desired_version[0];
	dest->desired_version[1] = src->desired_version[1];
	dest->desired_version[2] = src->desired_version[2];

	dest->desired_extensions_count = src->desired_extensions_count > MAX_VULKAN_EXTENSIONS ? 
		MAX_VULKAN_EXTENSIONS : src->desired_extensions_count; 
	const char *src_ext_ptr  = &src->desired_extensions[0][0];
	char *dest_ext_ptr = &dest->desired_extensions[0][0];
	for (size_t i = 0; i < src->desired_extensions_count; i++) {
		strcpy(dest_ext_ptr + i * VK_MAX_EXTENSION_NAME_SIZE, src_ext_ptr + i * VK_MAX_EXTENSION_NAME_SIZE);
	}

	dest->desired_device_features = src->desired_device_features;
}

void vulkan_config_log(const vulkan_config *vk_info) {
	printf(
		"vulkan: {\n"
		"  version: %lu.%lu.%lu,\n"
		"  extensions: [\n", 
		vk_info->desired_version[0], vk_info->desired_version[1], vk_info->desired_version[2]
	);
	for (size_t i = 0; i < vk_info->desired_extensions_count; i++) {
		printf("    %s,\n", vk_info->desired_extensions[i]);
	}
	printf(
		"  ]\n"
		"}\n"
	);
}

void init_default_queue_config(queue_family_config *queue_info) {
	strcpy(queue_info->name, "");
	queue_info->queue_count = 0;
	queue_info->queue_family_index = 0;
	for (size_t i = 0; i < MAX_QUEUE_COUNT; i++) {
		queue_info->priorities[i] = 1.0; 
	}
}	

void copy_queue_config(queue_family_config *dest, const queue_family_config *src) {
	strcpy(dest->name, src->name);
	dest->properties = src->properties;
	dest->queue_count = src->queue_count; 
	dest->queue_family_index = src->queue_family_index;
	memcpy(dest->priorities, src->priorities, sizeof(src->priorities));
}

void queue_config_log(const queue_family_config *queue_info) {
	printf(
		"queue: {\n"
		"  name: %s,\n"
		"  properties: VkQueueFamilyProperties {\n"
		"    queueFlags: %s\n"
		"    queueCount: %d\n" 
		"    timestampValidBits: %d\n"
		"    minImageTransferGranularity: (width = %d, height = %d, depth = %d)\n"
		"  }\n",
		queue_info->name,
		queue_flags_to_string(queue_info->properties.queueFlags), 
		queue_info->properties.queueCount, 
		queue_info->properties.timestampValidBits, 
		queue_info->properties.minImageTransferGranularity.width, queue_info->properties.minImageTransferGranularity.height, 
		queue_info->properties.minImageTransferGranularity.depth
	);
	printf(
		"  queue_family_index: %d\n"
		"  queue_count: %d\n"
		"  priorities: [ ",
		queue_info->queue_family_index,
		queue_info->queue_count
	);
	for (size_t i = 0; i < queue_info->queue_count; i++) {
		printf("%.2f ", queue_info->priorities[i]);
	}
	printf("]\n}\n");
}

