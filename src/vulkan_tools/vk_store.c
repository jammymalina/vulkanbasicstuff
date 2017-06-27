#include "vk_store.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <yaml.h>
#include "../logger/logger.h"
#include "parser.h"

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
	store->extensions_count = 0; 
	VkResult result = VK_SUCCESS; 

	result = vk->EnumerateInstanceExtensionProperties(NULL, &store->extensions_count, NULL);
	if (result != VK_SUCCESS || store->extensions_count == 0) {
		error_log("Could not get the Vulkan extensions.");	
		return false;
	} else {
		debug_log("Found %d extensions.", store->extensions_count);
	}
	if (store->extensions_count > MAX_VULKAN_EXTENSIONS) {
		error_log("Not enough space for extensions.");
		return false;
	}
	
	result = vk->EnumerateInstanceExtensionProperties(NULL, &store->extensions_count, &store->extensions[0]);
	if (result != VK_SUCCESS || store->extensions_count == 0) {
		error_log("Could not enumerate Vulkan extensions.");
		return false;
	} else {
		debug_log("Successfully enumerated %d extensions.", store->extensions_count);
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
	vulkan_config vk_info;

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

	return success;
}

