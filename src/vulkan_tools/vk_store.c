#include "vk_store.h"

#include <limits.h>
#include <stdlib.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <yaml.h>
#include "../logger/logger.h"

#define NO_APP_CONFIG 0 
#define INVALID_APP_CONFIG 1 
#define VALID_APP_CONFIG  2
#define PARSER_ERROR  3

static bool parse_long(const char *str, long *val) {
	char *temp;
	bool success = true;
	errno = 0;
	*val = strtol(str, &temp, 0);

	if (temp == str || *temp != '\0' ||  ((*val == LONG_MIN || *val == LONG_MAX) && errno == ERANGE))
		success = false;

	return success;
}

static int parse_version_index(const char *version) {
	if (strcmp(version, "major") == 0) {
		return 0;
	}
	if (strcmp(version, "minor") == 0) {
		return 1;
	}
	if (strcmp(version, "patch") == 0) {
		return 2; 
	}
	return -1;
}

static bool node_key_equals(yaml_node_t *node, const char *str) {
	if (node->type != YAML_SCALAR_NODE) 
		return false; 
	return strcmp((char*) node->data.scalar.value, str) == 0;
}

static size_t node_scalar_value_get(yaml_node_t *node, char *dest, size_t dest_size) {
	if (!node)
		return 0;
	if (node->type != YAML_SCALAR_NODE) 
		return 0;
    if (node->data.scalar.length >= dest_size)
		return 0;
	strncpy(dest, (char*) node->data.scalar.value, node->data.scalar.length);
	dest[node->data.scalar.length + 1] = '\0';	
	return node->data.scalar.length + 1;	
}	

static int parse_application_node(yaml_document_t *document, yaml_node_t *node) {
	if (node->type != YAML_MAPPING_NODE)
		return INVALID_APP_CONFIG;
	yaml_node_pair_t *iter;
	yaml_node_t *next_node;
	char value[STORE_FIELD_SIZE];
	for (iter = node->data.mapping.pairs.start; iter < node->data.mapping.pairs.top; iter++) {
		next_node = yaml_document_get_node(document, iter->key);
		if (next_node) {
			if (node_key_equals(next_node, "name")) {
				next_node = yaml_document_get_node(document, iter->value);
				size_t size = node_scalar_value_get(next_node, value, STORE_FIELD_SIZE);
				if (size == 0) {
					error_log("Error while parsing the name property, the prop may be invalid.");
				}
			} else if (node_key_equals(next_node, "title")) {
			} else if (node_key_equals(next_node, "version")) {
			} else if (node_key_equals(next_node, "engine")) {
			} else if (node_key_equals(next_node, "vulkan")) {
			}
		}
	}
	return VALID_APP_CONFIG;
}

static int parse_document(yaml_document_t *document) {
	yaml_node_t *node = yaml_document_get_root_node(document);
	if (!node) {
		return PARSER_ERROR; 
	}	
	yaml_node_t *next_node;
	yaml_node_pair_t *iter;	
	switch (node->type) {		
		case YAML_MAPPING_NODE:
			for (iter = node->data.mapping.pairs.start; iter < node->data.mapping.pairs.top; iter++) {
				next_node = yaml_document_get_node(document, iter->key); 
				if (next_node) {
					if (node_key_equals(next_node, "application")) {
						next_node = yaml_document_get_node(document, iter->value);
						if (!next_node) return PARSER_ERROR;
						return parse_application_node(document, next_node);	
					} 
				} else {
					debug_log("Couldn't find key node.");
				}
			}
			break; 
		default: 
			break;
		
	}	
	return NO_APP_CONFIG;
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
		error_log("Not enough space for extensions");
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

	yaml_document_t document; 
	while (!done) {
		if (!yaml_parser_load(&parser, &document)) {
			success = false;
			error_log("Parsing error."); 
			break;
		}
		done = !yaml_document_get_root_node(&document);
		
		if (!done) {
			int status = parse_document(&document);
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

