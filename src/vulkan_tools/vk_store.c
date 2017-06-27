#include "vk_store.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <yaml.h>
#include "../logger/logger.h"
#include "parser.h"
#include "utils.h"
#include "../utils/strcatcpy.h"

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

bool load_extensions(const vk_functions *vk, vk_store *store) {
	store->available_extensions_count = 0; 
	VkResult result = VK_SUCCESS; 

	result = vk->EnumerateInstanceExtensionProperties(NULL, &store->available_extensions_count, NULL);
	if (result != VK_SUCCESS || store->available_extensions_count == 0) {
		error_log("Could not get the Vulkan extensions.");	
		return false;
	} else {
		debug_log("Found %d extensions.", store->available_extensions_count);
	}
	if (store->available_extensions_count > MAX_VULKAN_EXTENSIONS) {
		error_log("Not enough space for extensions.");
		return false;
	}
	
	result = vk->EnumerateInstanceExtensionProperties(NULL, &store->available_extensions_count, 
		&store->available_extensions[0]);
	if (result != VK_SUCCESS || store->available_extensions_count == 0) {
		error_log("Could not enumerate Vulkan extensions.");
		return false;
	} else {
		debug_log("Successfully enumerated %d extensions.", store->available_extensions_count);
	}

	return true;
}

bool create_instance(const vk_functions *vk, vk_store *store, const char *config_filename) {
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
					// application_config_log(&app_info);
					// vulkan_config_log(&vk_info);
					break;
			}
		}

		yaml_document_delete(&document);
	}	
	
	debug_log("Done parsing the application config.");

	yaml_parser_delete(&parser);
	fclose(fh);

	if (!success) 
		return false; 
	
	copy_application_config(&store->application_info, &app_info);
	store->loaded_extensions_count = vk_info.desired_extensions_count;

	char **ext = (char**) malloc(store->loaded_extensions_count * sizeof(char*));
	if (*ext == NULL) {
		error_log("Problem with the memory allocation.");
		return false; 
	}
	if (store->loaded_extensions_count > 0) {
		for (size_t i = 0; i < store->loaded_extensions_count; i++) {
			if (!is_vulkan_extension_supported(store->available_extensions, store->available_extensions_count, 
				vk_info.desired_extensions[i]))
			{
				error_log("Extension %s not supported.", vk_info.desired_extensions[i]);
				return false; 
			}
			strcpy(store->loaded_extensions[i], vk_info.desired_extensions[i]);
			size_t ext_size = strlen(store->loaded_extensions[i]) + 1;  
			ext[i] = (char*) malloc(ext_size * sizeof(char));
			if (!ext[i]) {
				error_log("Problem with memory allocation.");
				return false;
			}
			strlcpy(ext[i], store->loaded_extensions[i], ext_size);
		}
	} 

	store->apiVersion = make_vulkan_version(vk_info.desired_version[0], 
		vk_info.desired_version[1], vk_info.desired_version[2]);
	
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
		.ppEnabledExtensionNames = (const char* const*) ext
	};

	VkResult result = vk->CreateInstance(&vk_instance_create_info, NULL, &store->instance);
	if (result != VK_SUCCESS || store->instance == VK_NULL_HANDLE) {
		error_log("Could not create Vulkan instance.");
		return false; 
	} else {
		debug_log("Successfully created the instance.");
	}
	for (size_t i = 0; i < store->loaded_extensions_count; i++)
		free(ext[i]); 
	free(ext);

	return true;
}

