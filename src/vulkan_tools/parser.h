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

int parse_document(yaml_document_t *document, application_config *app_info, vulkan_config *vk_info);

#endif // VULKAN_PARSER_H
