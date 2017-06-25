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

static bool parse_version(yaml_parser_t *parser, yaml_event_t *event, long values[3]) {
	bool prev_indices[] = { false, false, false };	
	if (event->type != YAML_MAPPING_START_EVENT) {
		return false;
	}
	yaml_event_delete(event);

	for (int i = 0; i < 3; i++) {
		if (!yaml_parser_parse(parser, event)) {
			return false;
		}	
		if (event->type == YAML_SCALAR_EVENT) {
			int index = parse_version_index((char*) event->data.scalar.value); 
			if (index == -1 || prev_indices[index]) {
				return false;
			}	
			yaml_event_delete(event);
			if (!yaml_parser_parse(parser, event)) {
				return false;
			}
			long v; 
			if (!parse_long((char*) event->data.scalar.value, &v)) {
				return false;
			}		
			values[index] = v; 
			prev_indices[index] = true;
			yaml_event_delete(event);
		} else if (event->type == YAML_MAPPING_END_EVENT) {
			break;
		} else {
			return false;
		}
	}	
	
	if (event->type != YAML_MAPPING_END_EVENT) {
		return false; 
	}
	yaml_event_delete(event);

	return yaml_parser_parse(parser, event);
}

static bool parse_empty(yaml_parser_t *parser, yaml_event_t *event) {
	yaml_event_delete(event);
	return !yaml_parser_parse(parser, event);
}

static bool parse_empty_mapping(yaml_parser_t *parser, yaml_event_t *event) {
	if (event->type != YAML_MAPPING_START_EVENT) {
		return false; 
	}
	int current_level = 0; 
	while (event->type != YAML_MAPPING_END_EVENT || current_level > 0) {
		if (!parse_empty(parser, event)) {
			return false;
		}
		if (event->type == YAML_MAPPING_START_EVENT) {
			current_level++;
		} else if (event->type == YAML_MAPPING_END_EVENT && current_level > 0) {
			current_level--;
		} 
	}
	return parse_empty(parser, event);
}

static bool parse_empty_sequence(yaml_parser_t *parser, yaml_event_t *event) {
	if (event->type != YAML_SEQUENCE_START_EVENT) {
		return false; 
	}
	int current_level = 0;
	while (event->type != YAML_SEQUENCE_END_EVENT || current_level > 0) {
		if (!parse_empty(parser, event)) {
			return false; 
		}
		if (event->type == YAML_SEQUENCE_START_EVENT) {
			current_level++;
		} else if (event->type == YAML_SEQUENCE_END_EVENT && current_level > 0) {
			current_level--;
		}
	} 
	return parse_empty(parser, event);
}

static bool parse_application(yaml_parser_t *parser, yaml_event_t *event, vk_store *store) {
	if (event->type != YAML_MAPPING_START_EVENT) {
		return false;
	}
	yaml_event_delete(event);
	if (!yaml_parser_parse(parser, event)) {
		return false;
	}

	bool finished = false;
	bool expect_value = false;
	char key[512];
	while (!finished) {
		switch(event.type) { 
			case YAML_NO_EVENT: 
			case YAML_STREAM_START_EVENT: 
			case YAML_STREAM_END_EVENT:
			case YAML_DOCUMENT_START_EVENT: 
			case YAML_DOCUMENT_END_EVENT: 
			case YAML_SEQUENCE_END_EVENT:
			case YAML_ALIAS_EVENT:
				return false;

			case YAML_SEQUENCE_START_EVENT:
				bool should_skip = true;
				if (expect_value) {
					if (strcmp())
				} 
				if (should_skip && !parse_empty_sequence(parser, event)) {
					return false;
				} 
				expect_value = false;
				break;
			case YAML_MAPPING_START_EVENT: 
				if (!parse_empty_mapping(parser, event)) {
					return false; 
				}
				expect_value = false;
				break;
			case YAML_SCALAR_EVENT: 
				bool should_skip = true;
				if (expect_value) {
					if (!parse_empty(parser, event)) {
						return false; 
					}
					expect_value = false;  
				} else {
					if (event->data.scalar.length >= 512) {
						error_log("Config file reading error - allocated space for key was not enough.");
						strcpy(key, ""); 
					}	
					expect_value = true;
				} 

				break;
			case YAML_MAPPING_END_EVENT:
				finished = true; 
				break;
		}

	}

	if (event->type != YAML_MAPPING_END_EVENT) {
		return false; 
	}
	yaml_event_delete(event);
	if (!yaml_parser_parse(parser, event)) {
		return false;
	}

	return true;
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
	
	yaml_event_t event;
	bool done = false;
	bool success = true;
	while (!done) {
		if (!yaml_parser_parse(&parser, &event)) {
			break;
		}
		
		if (strcmp((char*) event.data.scalar.value, "application") == 0) {
			success = parse_application(&parser, &event, store); 
			if (!success) {
				error_log("Invalid application config.");
			}
		}

		done = (event.type == YAML_STREAM_END_EVENT) || !success;
		yaml_event_delete(&event);
	}
	if (success)
		debug_log("Done parsing the application config.");
	else 
		error_log("Error while reading the application config.");
	yaml_parser_delete(&parser);
	fclose(fh);

	return success;
}

