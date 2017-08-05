#include <stdio.h>
#include "vulkan_functions/functions.h"
#include "vulkan_functions/function_loader.h"
#include "vulkan_tools/vk_store.h"
#include "vulkan_tools/utils.h"
#include "vulkan_tools/string_utils.h"
#include "logger/logger.h"

int main() {
	LIB_TYPE vulkan_lib = NULL;
	vk_functions vk;
	vk_store store;

	init_vulkan_store(&store);

	bool success = load_basic_vulkan_functions(&vulkan_lib, &vk) 
		&& init_store_from_config(&vk, &store, "./config/app.config")
		&& load_instance_vulkan_functions(&vk, store.instance, store.loaded_extensions);
	
	if (!success) {
		goto exit_program;
	}

	VkPhysicalDevice available_devices[MAX_VULKAN_DEVICES];
	uint32_t available_devices_count = 0;
	get_available_devices(&vk, store.instance, available_devices, &available_devices_count);	
	
	VkExtensionProperties device_extensions[MAX_VULKAN_EXTENSIONS];
	uint32_t extensions_count = 0;
	get_device_extensions(&vk, available_devices[0], device_extensions, &extensions_count);  
	
	debug_log("Successfully enumerated %d extensions:", extensions_count);
	for (size_t i = 0; i < extensions_count; i++) {
		debug_log("  %s, version: %d", device_extensions[i].extensionName, 
				device_extensions[i].specVersion);
	}

	VkPhysicalDeviceProperties physical_device_props; 
	VkPhysicalDeviceFeatures physical_device_features;
	get_device_features_and_props(&vk, available_devices[0], 
			&physical_device_features, &physical_device_props);
	physical_device_features_log(&physical_device_features);
	physical_device_props_log(&physical_device_props);	

	uint32_t queue_props_count = 0; 
	VkQueueFamilyProperties queue_props[MAX_QUEUE_PROPS];
	get_available_queue_props(&vk, available_devices[0], queue_props, &queue_props_count); 
	debug_log("Found %d queue family propertie(s)", queue_props_count);

	for (size_t i = 0; i < queue_props_count; i++) {
		queue_props_log(&queue_props[i]);
	}

	exit_program:
	if (vulkan_lib != NULL) { 
		FREE_LIB(vulkan_lib);
	}
}
