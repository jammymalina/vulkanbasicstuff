#ifndef VULKAN_PARSER_H
#define VULKAN_PARSER_H

#include <vulkan/vulkan.h>
#include <yaml.h>
#include <stdbool.h>
#include "vk_config.h"

#define NO_APP_CONFIG 0 
#define INVALID_APP_CONFIG 1 
#define VALID_APP_CONFIG 2
#define PARSER_ERROR 3

bool parse_unsigned_long(const char *str, unsigned long *val);
int version_index_get(const char *version);

bool node_key_equals(yaml_node_t *node, const char *str);
size_t node_scalar_value_get(yaml_node_t *node, char *dest, size_t dest_size);
int node_scalar_number_value_get(yaml_node_t *node, unsigned long *dest);

int parse_version(yaml_document_t *document, yaml_node_t *node, long version[3]);
int parse_string_sequence(yaml_document_t *document, yaml_node_t *node, 
	char *strings, size_t max_num_of_strings, size_t max_string_size, size_t *strings_count);

int parse_vulkan_node(yaml_document_t *document, yaml_node_t *node, vulkan_config *vk_info);
int parse_engine_node(yaml_document_t *document, yaml_node_t *node, application_config *app_info);
int parse_application_node(yaml_document_t *document, yaml_node_t *node, 
	application_config *app_info, vulkan_config *vk_info);

int parse_document(yaml_document_t *document, application_config *app_info, vulkan_config *vk_info);

#endif // VULKAN_PARSER_H
