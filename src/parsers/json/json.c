#include "json.h"

#include <ctype.h>

// json node

static bool add_json_node(json_tree_node *parent, json_tree_node *node) {
    if (parent->child_array_size <= parent->child_count) {
        size_t start_null_index = 0;
        if (parent->child_array_size == 0) {
            parent->child_array_size = 10;
            parent->children = (json_tree_node**) malloc(parent->child_array_size * sizeof(json_tree_node*));
        } else {
            parent->child_array_size = 2 * parent->child_count;
            parent->children = (json_tree_node**) realloc(parent->children, 
                parent->child_array_size * sizeof(json_tree_node*));
            start_null_index = parent->child_count;
        }
        if (parent->children == NULL) {
            parent->child_array_size = 0;
            parent->child_count = 0;
            return false; 
        }
        for (size_t i = start_null_index; i < parent->child_array_size; i++) {
            parent->children[i] = NULL;
        }
    }
    parent->children[parent->child_count] = node;
    parent->child_count++;
    node->parent = parent;
    return true;
}

static bool init_json_node(json_tree_node *node, json_tree_node *parent, const char *key) {
    size_t limit = JSON_MAX_STRING_LENGTH - 1 < strlen(key) ? JSON_MAX_STRING_LENGTH - 1 : strlen(key);
    memcpy(node->key, key, limit);
    node->key[limit] = '\0';

    node->child_count = 0;
    node->child_array_size = 0;
    node->children = NULL;
    node->parent = NULL;
    if (parent != NULL) {
        return add_json_node(parent, node);
    }
    return true;
}

static json_tree_node* create_json_node(json_tree_node *parent, const char *key) {
    json_tree_node *node = (json_tree_node*) malloc(sizeof(json_tree_node));
    if (node == NULL) {
        return NULL;
    }
    bool success = init_json_node(node, parent, key);
    if (!success) {
        return NULL;
    }
    return node;
}

static int index_of_json_node(json_tree_node *parent, json_tree_node *node) {
    int index = parent->child_count - 1;
    while (index >= 0 && parent->children[index] != node) {
        index--;
    }
    return index;
} 

static inline bool remove_json_node(json_tree_node *parent, json_tree_node *node) {
    int index = index_of_json_node(parent, node);
    if (index < 0)
        return false;
    node->parent = NULL;
    parent->child_count--;
    while (index < parent->child_count) {
        parent->children[index] = parent->children[index + 1];
        index++;
    }
    if (index < parent->child_array_size)
        parent->children[index] = NULL;
    return true;
}

static inline void destroy_json_node(json_tree_node *node) {
    if (node->parent != NULL) {
        remove_json_node(node->parent, node);
    }
    while (node->child_count > 0) {
        node->child_count--;
        node->children[node->child_count]->parent = NULL;
        destroy_json_node(node->children[node->child_count]);
        free(node->children[node->child_count]);
        node->children[node->child_count] = NULL;
    }
    free(node->children);
    node->children = NULL;
    free(node);
}

// string utils

static bool add_to_string(char *str, char c, size_t *index, size_t max_length) {
    if (*index >= max_length) {
        return false;
    }
    str[*index] = c;
    (*index)++;
    return true;
}

static bool parse_double(const char *str, double *val) {
	char *temp;
	*val = strtod(str, &temp);

	return !(temp == str || *temp != '\0');
}

static bool has_nchars_ahead(const char *json, size_t index, size_t n) {
    while (json[index] != '\0' && n > 0) {
        n--;
        index++;
    }
    return n == 0;
}

static bool has_string_ahead(const char *json, size_t index, const char *str) {
    while (json[index] != '\0' && str[index] != '\0' && json[index] == str[index]) {
        index++;
    }
    return str[index] == '\0';
}

static void jump_whitespace(const char *json, size_t *index) {
    while (json[*index] != '\0' && isspace(json[*index])) {
        (*index)++;
    }
}

static int next_token(const char *json, size_t *index) {
    jump_whitespace(json, index);

    if (json[*index] == '\0') {
        return JSON_TOKEN_NONE;
    }

    char c = json[*index];
    (*index)++;
    switch (c) {
        case '{':
            return JSON_TOKEN_CURLY_OPEN;
        case '}':
            return JSON_TOKEN_CURLY_CLOSE;
        case '[':
            return JSON_TOKEN_SQUARED_OPEN;
        case ']': 
            return JSON_TOKEN_SQUARED_CLOSE;
        case ',':
            return JSON_TOKEN_COMMA;
        case '"':
            return JSON_TOKEN_STRING;
        case '0': case '1': case '2': case '3': case '4': case '5': case '6': case '7': 
        case '8': case '9': case '-':
            return JSON_TOKEN_NUMBER;
        case ':': 
            return JSON_TOKEN_COLON;
    }
    (*index)--;

    if (has_nchars_ahead(json, *index, 5) && has_string_ahead(json, *index, "false")) {
        *index += 5; 
        return JSON_TOKEN_FALSE;
    }
    if (has_nchars_ahead(json, *index, 4) && has_string_ahead(json, *index, "true")) {
        *index += 4; 
        return JSON_TOKEN_TRUE;
    }
    if (has_nchars_ahead(json, *index, 4) && has_string_ahead(json, *index, "null")) {
        *index += 4; 
        return JSON_TOKEN_NULL;
    }

    return JSON_TOKEN_NONE;
}

static int look_ahead(const char *json, size_t index) {
    size_t tmp_index = index;
    return next_token(json, &tmp_index);
}

static void parse_string(json_parser *parser, char str[JSON_MAX_STRING_LENGTH]) {
    char c;

    jump_whitespace(parser->json, &parser->index);

    bool finished = false;
    parser->index++; // jump over "

    size_t str_index = 0;

    while (parser->json[parser->index] != '\0' && !finished) {
        c = parser->json[parser->index];
        parser->index++;        
        if (c == '"') {
            finished = true;
            break;
        } else if (c == '\\') {
            if (parser->json[parser->index] != '\0') {
                break;
            }
            parser->index++;
            if (c == '"') {
                add_to_string(str, '"', &str_index, JSON_MAX_STRING_LENGTH - 1);                
            } else if (c == '\\') {
                add_to_string(str, '\\', &str_index, JSON_MAX_STRING_LENGTH - 1);
            } else if (c == '/') {
                add_to_string(str, '/', &str_index, JSON_MAX_STRING_LENGTH - 1);
            } else if (c == 'b') {
                add_to_string(str, '\b', &str_index, JSON_MAX_STRING_LENGTH - 1);
            } else if (c == 'f') {
                add_to_string(str, '\f', &str_index, JSON_MAX_STRING_LENGTH - 1);
            } else if (c == 'n') {
                add_to_string(str, '\n', &str_index, JSON_MAX_STRING_LENGTH - 1);                
            } else if (c == 'r') {
                add_to_string(str, '\r', &str_index, JSON_MAX_STRING_LENGTH - 1);
            } else if (c == 't') {
                add_to_string(str, '\t', &str_index, JSON_MAX_STRING_LENGTH - 1);
            } else if (c == 'u') {
                if (!has_nchars_ahead(parser->json, parser->index, 4)) {
                    break;
                }
                add_to_string(str, JSON_UNKNOWN_CHAR, &str_index, JSON_MAX_STRING_LENGTH - 1);
                parser->index += 4;
            }
        } else {
            add_to_string(str, c, &str_index, JSON_MAX_STRING_LENGTH - 1);
        }
    }
    str[str_index] = '\0';
    return finished;
}

static bool parse_number(json_parser *parser) {
    jump_whitespace(parser->json, &parser->index);

    int last_index = 0;
    int str_length = (last_index + parser->index) + 1;

    parser->index = last_index + 1;
    return true;
}

static bool parse_object(json_parser *parser) {
    next_token(parser->json, &parser->index);

    bool done = false;
    while (!done) {
        int token = look_ahead(parser->json, parser->index);
        if (token == JSON_TOKEN_NONE) {
            return false;
        } else if (token == JSON_TOKEN_COMMA) {

        }
    }
}

void init_json_parse(json_parser *parser, char *json) {
    parser->index = 0; 
    parser->json = json;
}

bool parse_json(json_parser *parser, json_tree_node **root) {
    *root = create_json_node(NULL, "/");
    if (*root == NULL) {
        return false;
    }
}