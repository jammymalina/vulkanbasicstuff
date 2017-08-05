#include "vk_store.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <yaml.h>
#include "../logger/logger.h"
#include "utils.h"
#include "../utils/strcatcpy.h"

void init_vulkan_store(vk_store *store) {
	store->apiVersion = make_vulkan_version(1, 0, 0);
	store->loaded_extensions_count = 0;
	for (size_t i = 0; i < MAX_VULKAN_EXTENSIONS; i++) {
		for (size_t j = 0; j < VK_MAX_EXTENSION_NAME_SIZE; j++) {
			store->loaded_extensions[i][j] = '\0';
		}
	}
	init_default_application_config(&store->application_info);
}

void destroy_vulkan_store(vk_store *store) {
	// TODO: destructor 
}

static bool add_extensions(const vk_functions *vk, vk_store *store, const vulkan_config *vk_info, char ***ext_values) {
	store->loaded_extensions_count = vk_info->desired_extensions_count;

	bool success = true;
	char **ext = NULL;
	if (store->loaded_extensions_count > 0) {
		ext = (char**) malloc(store->loaded_extensions_count * sizeof(char*));
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
			ext[i] = (char*) malloc(ext_size * sizeof(char));
			if (!ext[i]) {
				error_log("Problem with memory allocation");
				return false;
			}
			nstrncpy(ext[i], store->loaded_extensions[i], ext_size);
		}
	}
	*ext_values = ext; 
}

static bool create_store_instance(const vk_functions *vk, vk_store *store, const vulkan_config *vk_info, char **ext) {
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
		error_log("Could not create Vulkan instance");
		return false; 
	} else {
		debug_log("Successfully created Vulkan instance.");
	}
}

bool init_store_from_config(const vk_functions *vk, vk_store *store, const char *config_filename) {
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
	
	char **ext;
	success = add_extensions(vk, store, &vk_info, &ext) && 
		create_store_instance(vk, store, &vk_info, ext);

	if (!success) {
		return false;
	}

	if (ext) {
		for (size_t i = 0; i < store->loaded_extensions_count; i++)
			free(ext[i]); 
		free(ext);
	}

	return true;
}

bool add_physical_device(const vk_functions *vk, vk_store *store) {
	VkPhysicalDevice available_devices[MAX_VULKAN_DEVICES];
	uint32_t available_devices_count = 0;
	bool success = get_available_devices(&vk, store->instance, available_devices, &available_devices_count);	
	
	if (!success) {
		error_log("Could not create physical device");
		return false;
	}

	
}

bool add_queue_balancer(const vk_functions *vk, vk_store *store) {
	create_queue_balancer(vk, &store->qb, store->physical_device);
}

