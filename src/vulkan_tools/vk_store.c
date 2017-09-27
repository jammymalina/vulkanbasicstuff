#include "vk_store.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <yaml.h>
#include "../vulkan_functions/function_loader.h"
#include "../logger/logger.h"
#include "utils.h"
#include "../utils/strcatcpy.h"

static bool create_store_instance(const vk_functions *vk, 
	vk_store *store, const vulkan_config *vk_info, char **ext) 
{
	store->apiVersion = make_vulkan_version(vk_info->desired_version[0], 
		vk_info->desired_version[1], vk_info->desired_version[2]);
	
	uint32_t engine_version = make_vulkan_version(store->application_info.engine.version[0],
			store->application_info.engine.version[1], store->application_info.engine.version[2]);
	uint32_t application_version = make_vulkan_version(store->application_info.version[0], 
			store->application_info.version[1], store->application_info.version[2]);
	VkApplicationInfo vk_application_info = {
		.sType              = VK_STRUCTURE_TYPE_APPLICATION_INFO,
		.pNext              = NULL,
		.pApplicationName   = store->application_info.name,
		.applicationVersion = application_version,
		.pEngineName        = store->application_info.engine.name,
		.engineVersion      = engine_version,
		.apiVersion         = store->apiVersion
	};

	VkInstanceCreateInfo vk_instance_create_info = {
		.sType                   = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
		.pNext                   = NULL,
		.flags                   = 0,
		.pApplicationInfo        = &vk_application_info,
		.enabledLayerCount       = 0,
		.ppEnabledLayerNames     = NULL,
		.enabledExtensionCount   = store->loaded_extensions_count,
		.ppEnabledExtensionNames = ext ? (const char* const*) ext : NULL
	};

	VkResult result = vk->CreateInstance(&vk_instance_create_info, NULL, &store->instance);
	if (result != VK_SUCCESS || store->instance == VK_NULL_HANDLE) {
		store->instance = NULL;
		error_log("Could not create Vulkan instance");
		return false; 
	}

	debug_log("Successfully created Vulkan instance.");
	return true;
}

static bool create_store_device(const vk_functions *vk, vk_store *store, char **dev_ext) {
	VkDeviceQueueCreateInfo queue_create_infos[MAX_QUEUE_FAMILIES];
	for (size_t i = 0; i < store->qb.queue_families_count; i++) {
		queue_create_infos[i].sType            = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
		queue_create_infos[i].pNext            = NULL;
		queue_create_infos[i].flags            = 0;
		queue_create_infos[i].queueFamilyIndex = store->qb.queue_families[i].queue_family_index;
		queue_create_infos[i].queueCount       = store->qb.queue_families[i].queue_count;
		queue_create_infos[i].pQueuePriorities = store->qb.queue_families[i].priorities;
	}

	VkDeviceCreateInfo device_create_info = {
		.sType                   = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO,
		.pNext                   = NULL,
		.flags                   = 0,
		.queueCreateInfoCount    = store->qb.queue_families_count,
		.pQueueCreateInfos       = queue_create_infos,
		.enabledLayerCount       = 0,
		.ppEnabledLayerNames     = NULL,
		.ppEnabledExtensionNames = dev_ext ? (const char* const*) dev_ext : NULL,
		.pEnabledFeatures        = &store->device_features
	};
	VkResult result = vk->CreateDevice(store->physical_device, &device_create_info, NULL, &store->device);
	if (result != VK_SUCCESS || (store->device == VK_NULL_HANDLE)) {
		store->device = NULL;
		error_log("Could not create Vulkan device");
		return false;
	}

	debug_log("Successfully created Vulkan device");
	return true;
}

static bool add_extensions(const vk_functions *vk, 
	vk_store *store, const vulkan_config *vk_info, char ***ext_values) 
{
	store->loaded_extensions_count = vk_info->desired_extensions_count;

	bool success = true;
	char **ext = NULL;
	if (store->loaded_extensions_count > 0) {
		ext = malloc(store->loaded_extensions_count * sizeof(char*));
		if (ext == NULL) {
			error_log("Problem with the memory allocation");
			return false; 
		}

		VkExtensionProperties available_extensions[MAX_VULKAN_EXTENSIONS];
		uint32_t available_extensions_count = 0;
		success = get_available_extensions(vk, available_extensions, &available_extensions_count);

		if (!success) {
			error_log("Error while getting avaliable extensions");
			return false; 
		}

		for (size_t i = 0; i < store->loaded_extensions_count; i++) {
			if (!is_vulkan_extension_supported(available_extensions, available_extensions_count, 
				vk_info->desired_extensions[i]))
			{
				error_log("Extension %s not supported", vk_info->desired_extensions[i]);
				return false; 
			}
			strcpy(store->loaded_extensions[i], vk_info->desired_extensions[i]);
			size_t ext_size = strlen(store->loaded_extensions[i]) + 1;  
			ext[i] = malloc(ext_size * sizeof(char));
			if (!ext[i]) {
				error_log("Problem with memory allocation");
				return false;
			}
			nstrncpy(ext[i], store->loaded_extensions[i], ext_size);
		}
	}
	*ext_values = ext; 
	return true;
}

static bool add_physical_device(const vk_functions *vk, vk_store *store, const vulkan_config *vk_info) {
	VkPhysicalDevice available_devices[MAX_VULKAN_DEVICES];
	uint32_t available_devices_count = 0;
	bool success = get_available_devices(vk, store->instance, available_devices, &available_devices_count);	
	
	if (!success) {
		error_log("Could not create physical device");
		return false;
	}

	VkPhysicalDevice desired_devices[MAX_VULKAN_DEVICES];
	uint32_t desired_devices_count = 0;

	for (size_t i = 0; i < available_devices_count; i++) {
		if (is_device_supported(vk, available_devices[i], vk_info->desired_device_extensions, 
			vk_info->desired_device_extensions_count, &vk_info->desired_device_features)) 
		{
			desired_devices[desired_devices_count] = available_devices[i];
			desired_devices_count++;		
		}

	}

	if (desired_devices_count == 0) {
		error_log("Couldn't find the device with the desired properties");
		return false; 
	}

	store->physical_device = desired_devices[0];
	return true;
}

static bool add_device_extensions(const vk_functions *vk, 
	vk_store *store, const vulkan_config *vk_info, char ***ext_values) 
{
	store->loaded_device_extensions_count = vk_info->desired_device_extensions_count;

	bool success = true;
	char **ext = NULL;
	if (store->loaded_device_extensions_count > 0) {
		ext = malloc(store->loaded_device_extensions_count * sizeof(char*));
		if (ext == NULL) {
			error_log("Problem with the memory allocation");
			return false; 
		}

		VkExtensionProperties available_extensions[MAX_VULKAN_EXTENSIONS];
		uint32_t available_extensions_count = 0;
		success = get_device_extensions(vk, 
			store->physical_device, available_extensions, &available_extensions_count); 

		if (!success) {
			error_log("Error while getting avaliable device extensions");
			return false; 
		}

		for (size_t i = 0; i < store->loaded_device_extensions_count; i++) {
			if (!is_vulkan_extension_supported(available_extensions, available_extensions_count, 
				vk_info->desired_device_extensions[i]))
			{
				error_log("Device extension %s not supported", vk_info->desired_device_extensions[i]);
				return false; 
			}
			strcpy(store->loaded_device_extensions[i], vk_info->desired_device_extensions[i]);
			size_t ext_size = strlen(store->loaded_device_extensions[i]) + 1;  
			ext[i] = malloc(ext_size * sizeof(char));
			if (!ext[i]) {
				error_log("Problem with memory allocation");
				return false;
			}
			nstrncpy(ext[i], store->loaded_device_extensions[i], ext_size);
		}
	}
	*ext_values = ext; 
	return true;
}

static bool add_device_features(const vk_functions *vk, vk_store *store, const vulkan_config *vk_info) {
	VkPhysicalDeviceFeatures available_features;
	get_device_features(vk, store->physical_device, &available_features);
	if (!are_vulkan_device_features_supported(&available_features, &vk_info->desired_device_features)) {
		error_log("Physical device features are not supported");
		return false;
	}
	store->device_features = vk_info->desired_device_features;
	return true;
}

static bool add_queue_balancer(const vk_functions *vk, vk_store *store) {
	create_queue_balancer(vk, &store->qb, store->physical_device);
	return true;
}

void init_vulkan_store(vk_store *store) {
	store->apiVersion = make_vulkan_version(1, 0, 0);
	store->loaded_extensions_count = 0;
	store->loaded_device_extensions_count = 0;
	for (size_t i = 0; i < MAX_VULKAN_EXTENSIONS; i++) {
		for (size_t j = 0; j < VK_MAX_EXTENSION_NAME_SIZE; j++) {
			store->loaded_extensions[i][j] = '\0';
			store->loaded_device_extensions[i][j] = '\0';
		}
	}

	store->instance        = VK_NULL_HANDLE; 
	store->physical_device = VK_NULL_HANDLE;
	store->device          = VK_NULL_HANDLE;

	set_vulkan_device_features(&store->device_features, VK_FALSE);
	init_default_application_config(&store->application_info);
	init_empty_queue_balancer(&store->qb);
}

void destroy_vulkan_store(const vk_functions *vk, vk_store *store) {
	if (store->device != VK_NULL_HANDLE) {
		vk->DestroyDevice(store->device, NULL);
		store->device = VK_NULL_HANDLE;	
	}
	if (store->instance != VK_NULL_HANDLE) {
		vk->DestroyInstance(store->instance, NULL);
		store->instance = VK_NULL_HANDLE;
	}
}

bool init_store_from_config(vk_functions *vk, vk_store *store, const char *config_filename) {
	init_vulkan_store(store);

	FILE *fh = fopen(config_filename, "rb");
	if (fh == NULL) {
		error_log("Failed to open file %s.", config_filename);
		return false;
	}

	yaml_parser_t parser;
	if (!yaml_parser_initialize(&parser)) {
		error_log("Failed to initialize the yaml parser.");
		fclose(fh); 
		return false;
	}

	yaml_parser_set_input_file(&parser, fh);
	
	bool success = true;			
	bool done = false;

	application_config app_info;
	init_default_application_config(&app_info);

	vulkan_config vk_info;
	init_default_vulkan_config(&vk_info);

	yaml_document_t document; 
	while (!done) {
		if (!yaml_parser_load(&parser, &document)) {
			success = false;
			error_log("Parsing error."); 
			break;
		}
		done = !yaml_document_get_root_node(&document);
		
		if (!done) {
			int status = parse_document(&document, &app_info, &vk_info);
			switch (status) {
				case PARSER_ERROR: 
					error_log("Parsing error.");
					done = true;
					success = false; 
					break;
				case INVALID_APP_CONFIG:
					error_log("Invalid configuration, check the app config file.");
					done = true; 
					success = false; 	
					break;
				case VALID_APP_CONFIG: 
					done = true; 
					debug_log("Successfully parsed the app config file.");
					application_config_log(&app_info);
					vulkan_config_log(&vk_info);
					break;
			}
		}

		yaml_document_delete(&document);
	}	
	
	debug_log("Done parsing the application config.");

	yaml_parser_delete(&parser);
	fclose(fh);

	if (!success) {
		return false;
	} 
	
	copy_application_config(&store->application_info, &app_info);
	
	char **ext, **dev_ext;
	success = add_extensions(vk, store, &vk_info, &ext) && 
		create_store_instance(vk, store, &vk_info, ext) && 
		load_instance_vulkan_functions(vk, store->instance, store->loaded_extensions, 
			store->loaded_extensions_count) &&
		add_physical_device(vk, store, &vk_info) &&
		add_device_extensions(vk, store, &vk_info, &dev_ext) &&
		add_device_features(vk, store, &vk_info) &&
		add_queue_balancer(vk, store) && 
		create_store_device(vk, store, dev_ext) && 
		load_device_level_functions(vk, store->device, store->loaded_device_extensions, 
			store->loaded_device_extensions_count) &&
		load_queues(vk, &store->qb, store->device);

	if (!success)
		return false;
	else 
		debug_log("Successfully created store");

	if (ext) {
		for (size_t i = 0; i < vk_info.desired_extensions_count; i++)
			free(ext[i]); 
		free(ext);
	}

	if (dev_ext) {
		for (size_t i = 0; i < vk_info.desired_device_extensions_count; i++)
			free(dev_ext[i]); 
		free(dev_ext);
	}

	return true;
}
