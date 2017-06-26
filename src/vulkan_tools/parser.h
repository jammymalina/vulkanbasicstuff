#include <vulkan/vulkan.h>
#include <yaml.h>
#include <string.h>
#include <stdbool.h>
#include <limits.h>
#include <stdlib.h> 
#include <errno.h>
#include "../logger/logger.h"
#include "vulkan_limits.h"

#define NO_APP_CONFIG 0 
#define INVALID_APP_CONFIG 1 
#define VALID_APP_CONFIG 2
#define PARSER_ERROR 3

static bool parse_unsigned_long(const char *str, unsigned long *val) {
	char *temp;
	errno = 0;
	*val = strtoul(str, &temp, 0);

	return !(temp == str || *temp != '\0' || ((*val == 0 || *val == ULONG_MAX) && errno == ERANGE) || 
		*val > LONG_MAX);
}

static int version_index_get(const char *version) {
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
	if (node->type != YAML_SCALAR_NODE) 
		return 0;
    if (node->data.scalar.length >= dest_size)
		return 0;
	strncpy(dest, (char*) node->data.scalar.value, node->data.scalar.length);
	dest[node->data.scalar.length + 1] = '\0';	
	return node->data.scalar.length + 1;	
}	

static int node_scalar_number_value_get(yaml_node_t *node, unsigned long *dest) {
	if (node->type != YAML_SCALAR_NODE) 
		return INVALID_APP_CONFIG; 
	bool success = parse_unsigned_long((char*) node->data.scalar.value, dest);
	return success ? VALID_APP_CONFIG : INVALID_APP_CONFIG;
}

static int parse_version(yaml_document_t *document, yaml_node_t *node, long version[3]) {
	if (node->type != YAML_MAPPING_NODE)
		return INVALID_APP_CONFIG;
	version[0] = version[1] = version[3] = -1; 
	yaml_node_t *next_node;
	yaml_node_pair_t *iter;
	for (iter = node->data.mapping.pairs.start; iter < node->data.mapping.pairs.top; iter++) {
		next_node = yaml_document_get_node(document, iter->key);
		if (next_node) {
			if (node_key_equals(next_node, "major") || 
				node_key_equals(next_node, "minor") || 
				node_key_equals(next_node, "patch")) 
			{
				int index = version_index_get((char*) next_node->data.scalar.value); 
				if (index == -1) 
					return INVALID_APP_CONFIG;
				next_node = yaml_document_get_node(document, iter->value);
				if (!next_node)
					return PARSER_ERROR;
				unsigned long value; 
				int status = node_scalar_number_value_get(next_node, &value); 		
				if (status != VALID_APP_CONFIG) {
					return status;
				}	
				version[index] = value;
			} else {
				return INVALID_APP_CONFIG;
			}
		} else {
			return PARSER_ERROR;
		}
	}
	return VALID_APP_CONFIG;
}

static int parse_string_sequence(yaml_document_t *document, yaml_node_t *node, 
	char *strings, size_t max_num_of_strings, size_t max_string_size, size_t *strings_count)
{
	if (node->type != YAML_SEQUENCE_NODE)
		return INVALID_APP_CONFIG;
	size_t i = 0;
	yaml_node_t *next_node;
	yaml_node_item_t *iter;	
	for (iter = node->data.sequence.items.start; iter < node->data.sequence.items.top; iter++) {
		next_node = yaml_document_get_node(document, *iter);
		if (!next_node)
			return PARSER_ERROR;
		if (i > max_num_of_strings)
			return INVALID_APP_CONFIG; 
		size_t size = node_scalar_value_get(next_node, strings + i * max_string_size, max_string_size);
		if (size == 0) {
			error_log("Error while parsing the name property, the prop is invalid.");
			return INVALID_APP_CONFIG;
		}
		i++;
	}
	*strings_count = i;
	return VALID_APP_CONFIG;
}

static int parse_vulkan_node(yaml_document_t *document, yaml_node_t *node, vulkan_config *vk_info) {
	if (node->type != YAML_MAPPING_NODE) 
		return INVALID_APP_CONFIG;
	char extensions_values[MAX_VULKAN_EXTENSIONS][VK_MAX_EXTENSION_NAME_SIZE];
	long version_value[3];
	yaml_node_t *next_node;
	yaml_node_pair_t *iter;	
	for (iter = node->data.mapping.pairs.start; iter < node->data.mapping.pairs.top; iter++) {
		next_node = yaml_document_get_node(document, iter->key);
		if (next_node) {
			if (node_key_equals(next_node, "version")) {
				next_node = yaml_document_get_node(document, iter->value);
				if (!next_node) {
					error_log("Parser error, location: the vulkan version node.");
					return PARSER_ERROR;
				}
				int status = parse_version(document, next_node, version_value);
				if (status != VALID_APP_CONFIG) {
					if (status == INVALID_APP_CONFIG)
						error_log("Error while parsing the vulkan version property, the prop is invalid.");
					else 
						error_log("Parser error, location: the vulkan version node.");
					return status;
				}
				vk_info->desired_version[0] = version_value[0] != -1 ? 
					version_value[0] : vk_info->desired_version[0]; 
				vk_info->desired_version[1] = version_value[1] != -1 ? 
					version_value[1] : vk_info->desired_version[1]; 
				vk_info->desired_version[2] = version_value[0] != -1 ? 
					version_value[2] : vk_info->desired_version[2]; 
			} else if (node_key_equals(next_node, "extensions")) {
				next_node = yaml_document_get_node(document, iter->value);
				if (!next_node) {
					error_log("Parser error, location: the vulkan extensions node.");
					return PARSER_ERROR;
				}
				size_t extensions_values_count;
				int status = parse_string_sequence(document, next_node,&extensions_values[0][0], 
					MAX_VULKAN_EXTENSIONS, VK_MAX_EXTENSION_NAME_SIZE, &extensions_values_count);
				if (status != VALID_APP_CONFIG) {
					if (status == INVALID_APP_CONFIG)
						error_log("Error while parsing the vulkan version property, the prop is invalid.");
					else 
						error_log("Parser error, location: the vulkan version node.");
					return status;
				}
				vk_info->desired_extensions_count = extensions_values_count; 
				for (size_t i = 0; i < vk_info->desired_extensions_count; i++) {
					strcpy(vk_info->desired_extensions[i], extensions_values[i]);
				}
			}
		} else {
			error_log("Parser error, location: the engine node.");
			return PARSER_ERROR;
		}
	}
	return VALID_APP_CONFIG;
}

static int parse_engine_node(yaml_document_t *document, yaml_node_t *node, application_config *app_info) {
	if (node->type != YAML_MAPPING_NODE) 
		return INVALID_APP_CONFIG;
	char value[STORE_FIELD_SIZE];
	long version_value[3];
	yaml_node_t *next_node;
	yaml_node_pair_t *iter;
	for (iter = node->data.mapping.pairs.start; iter < node->data.mapping.pairs.top; iter++) {
		next_node = yaml_document_get_node(document, iter->key);
		if (next_node) {
			if (node_key_equals(next_node, "name")) {
				next_node = yaml_document_get_node(document, iter->value);
				if (!next_node) {
					error_log("Parser error, location: the engine name node.");
					return PARSER_ERROR;
				}
				size_t size = node_scalar_value_get(next_node, value, STORE_FIELD_SIZE);
				if (size == 0) {
					error_log("Error while parsing the name property, the prop is invalid.");
					return INVALID_APP_CONFIG;
				}
				strcpy(app_info->engine.name, value);
			} else if (node_key_equals(next_node, "version")) {
				next_node = yaml_document_get_node(document, iter->value);
				if (!next_node) {
					error_log("Parser error, location: the engine version node.");
					return PARSER_ERROR;
				}
				int status = parse_version(document, next_node, version_value);
				if (status != VALID_APP_CONFIG) {
					if (status == INVALID_APP_CONFIG)
						error_log("Error while parsing the engine version property, the prop is invalid.");
					else 
						error_log("Parser error, location: the engine version node.");
					return status;
				}
				app_info->engine.version[0] = version_value[0] != -1 ? 
					version_value[0] : app_info->engine.version[0]; 
				app_info->engine.version[1] = version_value[1] != -1 ? 
					version_value[1] : app_info->engine.version[1]; 
				app_info->engine.version[2] = version_value[2] != -1 ? 
					version_value[2] : app_info->engine.version[2]; 
			}
		} else {
			error_log("Parser error, location: the engine node.");
			return PARSER_ERROR;
		}
	}
	return VALID_APP_CONFIG;
}

static int parse_application_node(yaml_document_t *document, yaml_node_t *node, 
	application_config *app_info, vulkan_config *vk_info) 
{
	if (node->type != YAML_MAPPING_NODE)
		return INVALID_APP_CONFIG;
	yaml_node_pair_t *iter;
	yaml_node_t *next_node;
	char value[STORE_FIELD_SIZE];
	long version_value[3];
	for (iter = node->data.mapping.pairs.start; iter < node->data.mapping.pairs.top; iter++) {
		next_node = yaml_document_get_node(document, iter->key);
		if (next_node) {
			if (node_key_equals(next_node, "name")) {
				next_node = yaml_document_get_node(document, iter->value);
				if (!next_node) {
					error_log("Parser error, location: the application name node.");
					return PARSER_ERROR;
				}
				size_t size = node_scalar_value_get(next_node, value, STORE_FIELD_SIZE);
				if (size == 0) {
					error_log("Error while parsing the name property, the prop is invalid.");
					return INVALID_APP_CONFIG;
				}
				strcpy(app_info->name, value);
			} else if (node_key_equals(next_node, "title")) {
				next_node = yaml_document_get_node(document, iter->value);
				if (!next_node) {
					error_log("Parser error, location: the application title node.");
					return PARSER_ERROR;
				}
				size_t size = node_scalar_value_get(next_node, value, STORE_FIELD_SIZE);
				if (size == 0) {
					error_log("Error while parsing the title property, the prop is invalid.");
					return INVALID_APP_CONFIG;
				}
				strcpy(app_info->title, value);
			} else if (node_key_equals(next_node, "version")) {
				next_node = yaml_document_get_node(document, iter->value);
				if (!next_node) {
					error_log("Parser error, location: the application version node.");
					return PARSER_ERROR;
				}
				int status = parse_version(document, next_node, version_value);
				if (status != VALID_APP_CONFIG) {
					error_log("Error while parsing the application version property, the prop is invalid.");
					return status;
				}
				app_info->version[0] = version_value[0] != -1 ? version_value[0] : app_info->version[0]; 
				app_info->version[1] = version_value[1] != -1 ? version_value[1] : app_info->version[1]; 
				app_info->version[2] = version_value[2] != -1 ? version_value[2] : app_info->version[2]; 
			} else if (node_key_equals(next_node, "engine")) {
				next_node = yaml_document_get_node(document, iter->value);
				if (!next_node) {
					error_log("Parser error, location: the application engine node.");
					return PARSER_ERROR;
				}
				int status = parse_engine_node(document, next_node, app_info);
				if (status != VALID_APP_CONFIG) {
					error_log("Error while parsing the engine property.");
					return status;
				}
			} else if (node_key_equals(next_node, "vulkan")) {
				next_node = yaml_document_get_node(document, iter->value);
				if (!next_node) {
					error_log("Parser error, location: the application vulkan node.");
					return PARSER_ERROR;
				}
				int status = parse_vulkan_node(document, next_node, vk_info);
				if (status != VALID_APP_CONFIG) {
					error_log("'Error while parsing the vulkan property.");
					return status;
				}
			}
		} else {
			error_log("Parser error, location: the application node.");
			return PARSER_ERROR;
		}
	}
	return VALID_APP_CONFIG;
}

static int parse_document(yaml_document_t *document, application_config *app_info, vulkan_config *vk_info) {
	yaml_node_t *node = yaml_document_get_root_node(document);
	if (!node) {
		return PARSER_ERROR; 
	}	
	yaml_node_t *next_node;
	yaml_node_pair_t *iter;	
	if (node->type == YAML_MAPPING_NODE) {
		for (iter = node->data.mapping.pairs.start; iter < node->data.mapping.pairs.top; iter++) {
			next_node = yaml_document_get_node(document, iter->key); 
			if (next_node) {
				if (node_key_equals(next_node, "application")) {
					next_node = yaml_document_get_node(document, iter->value);
					if (!next_node) {
						error_log("Parser error, location: the application value node.");
						return PARSER_ERROR;
					}
					return parse_application_node(document, next_node, app_info, vk_info);	
				} 
			} else {
				error_log("Parser error, location: the document.");
				return PARSER_ERROR;
			}
		}
	} 		
	return NO_APP_CONFIG;
}
