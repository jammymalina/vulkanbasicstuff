#include "vk_store.h"

#include <limits.h>
#include <stdlib.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <yaml.h>
#include "../logger/logger.h"

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

static bool parse_node(yaml_document_t *document, yaml_node_t *node) {
	static int x = 0; 
	x++; 
	int node_n = x;	

	bool success = true;

	yaml_node_t *next_node;

	debug_log("START NODE");		
	switch (node->type) {
		case YAML_NO_NODE: 
			debug_log("Empty node: %d.", node_n); 
			break; 
		case YAML_SCALAR_NODE: 
			debug_log("Scalar node: %d, %s.", node_n, node->data.scalar.value); 
			break;
		case YAML_SEQUENCE_NODE:
			debug_log("Sequence node: %d.", node_n);
			yaml_node_item_t *iter;	
			for (iter = node->data.sequence.items.start; iter < node->data.sequence.items.top; iter++) {
				next_node = yaml_document_get_node(document, *iter);
				if (next_node) {
					parse_node(document, next_node);
				}
			}
			break; 	
		case YAML_MAPPING_NODE:	
			debug_log("Mapping node: %d, %d.", node_n);
			yaml_node_pair_t *piter;	
			for (piter = node->data.mapping.pairs.start; piter < node->data.mapping.pairs.top; piter++) {
				next_node = yaml_document_get_node(document, piter->key); 
				if (next_node) {
					debug_log("Key: "); 
					parse_node(document, next_node);
				} else {
					debug_log("Couldn't find key node.");
				}

				next_node = yaml_document_get_node(document, piter->value); 
				if (next_node) {
					debug_log("Value: "); 
					parse_node(document, next_node);
				} else {
					debug_log("Couldn't find value node.");
				}
			}
			break; 
		default: 
			debug_log("Unknown node for whatever reason."); 
			break;
		
	}	

	debug_log("END NODE");

	return success; 
}

static bool parse_document(yaml_document_t *document) {
	debug_log("NEW DOCUMENT"); 
	bool success = parse_node(document, yaml_document_get_root_node(document));
	debug_log("END DOCUMENT");
   return success; 	
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
	yaml_document_t document; 			

	
	bool done = false; 
	while (!done) {
		if (!yaml_parser_load(&parser, &document)) {
			success = false;
			goto parsingcleanup;
		}
		done = !yaml_document_get_root_node(&document);
		
		if (!done) {
			parse_document(&document);
		}

		yaml_document_delete(&document);
	}	
	
	yaml_document_delete(&document);

	parsingcleanup:
	if (success)
		debug_log("Done parsing the application config.");
	else 
		error_log("Error while reading the application config.");

	yaml_parser_delete(&parser);
	fclose(fh);

	return success;
}

