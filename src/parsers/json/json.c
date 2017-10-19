#include "json.h"

#include <ctype.h>
#include <string.h>
#include <stdio.h>

// parser struct 

typedef struct {
    char *json;
    size_t index;
} json_parser;

static bool init_json_parse(json_parser *parser, const char *json);

// json node

static bool add_json_node(json_tree_node *parent, json_tree_node *node);
static bool init_json_node(json_tree_node *node, json_tree_node *parent, const char *key, 
    json_value_type value_type);
static json_tree_node* create_json_node(json_tree_node *parent, const char *key, json_value_type value_type);
static int index_of_json_node(json_tree_node *parent, json_tree_node *node);
static bool remove_json_node(json_tree_node *parent, json_tree_node *node);

// string utils 

static bool parse_double(const char *str, double *val);
static bool add_char_to_str(char *str, char c, size_t *index, size_t max_length);
static int char_index_of(const char *str, char c);
static bool size_to_str(size_t num, char str[JSON_MAX_STRING_LENGTH]);

// pointer utils

static const char* json_value_type_to_string(json_value_type type);
static bool append_json_pointer(char dest[JSON_POINTER_PRINT_MAX_LENGTH], const char *key);
static void pop_json_pointer(char dest[JSON_POINTER_PRINT_MAX_LENGTH]);
static void print_all_json_pointers_r(const json_tree_node *node, char str[JSON_POINTER_PRINT_MAX_LENGTH]);
static const json_tree_node* get_json_node_r(const json_tree_node *node, const char *json_pointer);

// parser utils

static size_t get_last_index_of_number(const char *json, size_t index);
static bool has_string_ahead(const char *json, size_t index, const char *str);
static bool has_nchars_ahead(const char *json, size_t index, size_t n);
static int look_ahead(const char *json, size_t index);
static void jump_whitespace(const char *json, size_t *index);
static int next_token(const char *json, size_t *index);

static bool parse_string(json_parser *parser, char str[JSON_MAX_STRING_LENGTH]);
static bool parse_number(json_parser *parser, double *value);
static bool parse_array(json_parser *parser, json_tree_node *parent);
static bool parse_object(json_parser *parser, json_tree_node *parent);
static bool parse_value(json_parser *parser, json_tree_node **node, const char *key);

bool init_json_node(json_tree_node *node, json_tree_node *parent, const char *key, json_value_type value_type) {
    size_t limit = JSON_MAX_STRING_LENGTH - 1 < strlen(key) ? JSON_MAX_STRING_LENGTH - 1 : strlen(key);
    memcpy(node->key, key, limit);
    node->key[limit] = '\0';
    node->value.type = value_type;

    node->child_count = 0;
    node->child_array_size = 0;
    node->children = NULL;
    node->parent = NULL;
    if (parent != NULL) {
        return add_json_node(parent, node);
    }
    return true;
}

bool add_json_node(json_tree_node *parent, json_tree_node *node) {
    if (parent->child_array_size <= parent->child_count) {
        size_t start_null_index = 0;
        if (parent->child_array_size == 0) {
            parent->child_array_size = 10;
            parent->children = malloc(parent->child_array_size * sizeof(json_tree_node*));
        } else {
            parent->child_array_size = 2 * parent->child_count;
            parent->children =  realloc(parent->children, 
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

json_tree_node* create_json_node(json_tree_node *parent, const char *key, json_value_type value_type) {
    json_tree_node *node = malloc(sizeof(json_tree_node));
    if (node == NULL) {
        return NULL;
    }
    bool success = init_json_node(node, parent, key, value_type);
    if (!success) {
        return NULL;
    }
    return node;
}

int index_of_json_node(json_tree_node *parent, json_tree_node *node) {
    int index = parent->child_count - 1;
    while (index >= 0 && parent->children[index] != node) {
        index--;
    }
    return index;
} 

bool remove_json_node(json_tree_node *parent, json_tree_node *node) {
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

void destroy_json_node(json_tree_node *node) {
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

bool add_char_to_str(char *str, char c, size_t *index, size_t max_length) {
    if (*index >= max_length) {
        return false;
    }
    str[*index] = c;
    (*index)++;
    return true;
}

bool parse_double(const char *str, double *val) {
	char *temp;
	*val = strtod(str, &temp);
	return !(temp == str || *temp != '\0');
}

int char_index_of(const char *str, char c) {
    for (size_t i = 0; str[i] != '\0'; i++) {
        if (str[i] == c) {
            return (int) i;
        }
    }
    return -1;
}

bool size_to_str(size_t num, char str[JSON_MAX_STRING_LENGTH]) {
    size_t i = num;
    size_t char_count = 0;
    do {
        char c = (i % 10) + '0';
        i /= 10; 
        str[char_count] = c;
        char_count++;
    } while(i > 0 && char_count < JSON_MAX_STRING_LENGTH - 1);
    str[char_count] = '\0';
    return i == 0;
}

// pointer utils

bool append_json_pointer(char dest[JSON_POINTER_PRINT_MAX_LENGTH], const char *key) {
    size_t pointer_index = strlen(dest);
    size_t i;
    for (i = 0; i < strlen(key) && pointer_index < JSON_POINTER_PRINT_MAX_LENGTH - 1; i++, pointer_index++) {
        dest[pointer_index] = key[i];
    }
    dest[pointer_index] = '\0';
    return i == strlen(key);
}

void pop_json_pointer(char dest[JSON_POINTER_PRINT_MAX_LENGTH]) {
    size_t i = strlen(dest) - 1;
    while (dest[i] != '/') {
        dest[i] = '\0';
        if (i == 0) {
            break;
        }
        i--;
    }
    if (i > 0) {
        dest[i] = '\0';
    }
}

const char* json_value_type_to_string(json_value_type type) {
    switch (type) {
        case JSON_VALUE_NULL:
            return "null";
        case JSON_VALUE_NUMBER:
            return "number";
        case JSON_VALUE_ARRAY:
            return "array";
        case JSON_VALUE_OBJECT:
            return "object";
        case JSON_VALUE_BOOLEAN:
            return "boolean";
        case JSON_VALUE_STRING:
            return "string";
        case JSON_VALUE_NONE:
            return "none";
        default:
            return "none";
    }
}

// parser utils


size_t get_last_index_of_number(const char *json, size_t index) {
    const char *number_chars = "0123456789+-.eE";
    size_t last_index = index;
    for (last_index = index; json[last_index] != '\0'; last_index++) {
        if (char_index_of(number_chars, json[last_index]) == -1)
            break;
    }
    return last_index - 1;
}

bool has_nchars_ahead(const char *json, size_t index, size_t n) {
    while (json[index] != '\0' && n > 0) {
        n--;
        index++;
    }
    return n == 0;
}

bool has_string_ahead(const char *json, size_t index, const char *str) {
    while (json[index] != '\0' && str[index] != '\0' && json[index] == str[index]) {
        index++;
    }
    return str[index] == '\0';
}

void jump_whitespace(const char *json, size_t *index) {
    while (json[*index] != '\0' && isspace(json[*index])) {
        (*index)++;
    }
}

int next_token(const char *json, size_t *index) {
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

int look_ahead(const char *json, size_t index) {
    size_t tmp_index = index;
    return next_token(json, &tmp_index);
}

bool parse_string(json_parser *parser, char str[JSON_MAX_STRING_LENGTH]) {
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
                add_char_to_str(str, '"', &str_index, JSON_MAX_STRING_LENGTH - 1);                
            } else if (c == '\\') {
                add_char_to_str(str, '\\', &str_index, JSON_MAX_STRING_LENGTH - 1);
            } else if (c == '/') {
                add_char_to_str(str, '/', &str_index, JSON_MAX_STRING_LENGTH - 1);
            } else if (c == 'b') {
                add_char_to_str(str, '\b', &str_index, JSON_MAX_STRING_LENGTH - 1);
            } else if (c == 'f') {
                add_char_to_str(str, '\f', &str_index, JSON_MAX_STRING_LENGTH - 1);
            } else if (c == 'n') {
                add_char_to_str(str, '\n', &str_index, JSON_MAX_STRING_LENGTH - 1);                
            } else if (c == 'r') {
                add_char_to_str(str, '\r', &str_index, JSON_MAX_STRING_LENGTH - 1);
            } else if (c == 't') {
                add_char_to_str(str, '\t', &str_index, JSON_MAX_STRING_LENGTH - 1);
            } else if (c == 'u') {
                if (!has_nchars_ahead(parser->json, parser->index, 4)) {
                    break;
                }
                add_char_to_str(str, JSON_UNKNOWN_CHAR, &str_index, JSON_MAX_STRING_LENGTH - 1);
                parser->index += 4;
            }
        } else {
            add_char_to_str(str, c, &str_index, JSON_MAX_STRING_LENGTH - 1);
        }
    }
    str[str_index] = '\0';
    return finished;
}

bool parse_number(json_parser *parser, double *value) {
    *value = 0.0;

    jump_whitespace(parser->json, &parser->index);

    int last_index = get_last_index_of_number(parser->json, parser->index);
    char tmp = parser->json[last_index + 1];
    parser->json[last_index + 1] = '\0';

    bool success = parse_double(parser->json + parser->index, value);

    parser->json[last_index + 1] = tmp;        
    parser->index = last_index + 1;

    return success;
}

bool parse_object(json_parser *parser, json_tree_node *parent) {
    next_token(parser->json, &parser->index);

    bool done = false;
    bool success = true;
    char key[JSON_MAX_STRING_LENGTH];

    json_tree_node *node;
    
    while (!done) {
        int token = look_ahead(parser->json, parser->index);
        switch (token) {
            case JSON_TOKEN_NONE: 
                return false;
            case JSON_TOKEN_COMMA:
                next_token(parser->json, &parser->index);
                break;
            case JSON_TOKEN_CURLY_CLOSE:
                next_token(parser->json, &parser->index);
                return true;
            default:
                // name
                success = parse_string(parser, key);
                if (!success) {
                    return false;
                }
                // :
                token = next_token(parser->json, &parser->index);
                if (token != JSON_TOKEN_COLON) {
                    return false;
                }
                // value
                success = parse_value(parser, &node, key) && add_json_node(parent, node);
                if (!success) {
                    return false;
                }
                break;
        }
    }

    return true;
}

bool parse_array(json_parser *parser, json_tree_node *parent) {
    next_token(parser->json, &parser->index);

    size_t i = 0;
    char key[JSON_MAX_STRING_LENGTH];
    bool success = false;
    
    json_tree_node *node;
    bool finished = false; 

    while (!finished) {
        int token = look_ahead(parser->json, parser->index);
        switch (token) {
            case JSON_TOKEN_NONE:
                return false;
            case JSON_TOKEN_COMMA:
                next_token(parser->json, &parser->index);
                break;
            case JSON_TOKEN_SQUARED_CLOSE:
                next_token(parser->json, &parser->index);
                finished = true;
                break;
            default:
                success = size_to_str(i, key) &&
                    parse_value(parser, &node, key) &&
                    add_json_node(parent, node);
                if (!success) {
                    return false;
                }
                i++;
                break;
        }
    }   

    return true;
}

bool parse_value(json_parser *parser, json_tree_node **node, const char *key) {
    double num;
    char str[JSON_MAX_STRING_LENGTH];
    bool success;
    int token = look_ahead(parser->json, parser->index);
    *node = create_json_node(NULL, key, JSON_VALUE_NONE);
    if (*node == NULL) {
        return false;
    }         
    switch (token) {
        case JSON_TOKEN_STRING:
            success = parse_string(parser, str);
            if (!success) {
                return false;
            }
            (*node)->value.type = JSON_VALUE_STRING;
            strcpy((*node)->value.str, str);
            return true;
        case JSON_TOKEN_NUMBER:
            success = parse_number(parser, &num);
            if (!success) {
                return false;
            }
            (*node)->value.type = JSON_VALUE_NUMBER;            
            (*node)->value.num = num;
            return true;
        case JSON_TOKEN_CURLY_OPEN:
            (*node)->value.type = JSON_VALUE_OBJECT;    
            return parse_object(parser, *node);
        case JSON_TOKEN_SQUARED_OPEN:
            (*node)->value.type = JSON_VALUE_ARRAY;
            return parse_array(parser, *node);
        case JSON_TOKEN_TRUE:
        case JSON_TOKEN_FALSE:
            (*node)->value.type = JSON_VALUE_BOOLEAN;    
            (*node)->value.b = token == JSON_TOKEN_TRUE;
            next_token(parser->json, &parser->index);
            return true;
        case JSON_TOKEN_NULL:
            (*node)->value.type = JSON_VALUE_NULL;    
            next_token(parser->json, &parser->index);
            return true;
        case JSON_TOKEN_NONE:
            break;
    }
    return false;
}

 bool init_json_parse(json_parser *parser, const char *json) {
    parser->index = 0; 
    
    size_t len = strlen(json);
    parser->json = malloc(len + 1);
    if (parser->json == NULL) {
        return false;
    }
    strcpy(parser->json, json);
    return true;
}

const json_tree_node* get_json_node_r(const json_tree_node *node, const char *json_pointer) {
    char key[JSON_MAX_STRING_LENGTH];
    const char *p = json_pointer;
    p++;
    size_t i = 0;
    while (i < JSON_MAX_STRING_LENGTH - 1 && *p != '/' && *p != '\0') {
        key[i] = *p;
        i++;
        p++;
    }
    key[i] = '\0';
    if (*p != '/' && *p != '\0') {
        return NULL;
    }
    for (size_t j = 0; j < node->child_count; j++) {
        if (strcmp(node->children[j]->key, key) == 0) {
            return *p == '\0' ? node->children[j] : get_json_node_r(node->children[j], p);
        }
    }
    return NULL;
}

const json_tree_node* get_json_node(const json_tree_node *root, const char *json_pointer) {
    if (strcmp(json_pointer, "/") == 0) {
        return root;
    }
    return get_json_node_r(root, json_pointer);
}

void print_all_json_pointers_r(const json_tree_node *node, char str[JSON_POINTER_PRINT_MAX_LENGTH]) {
    append_json_pointer(str, node->key);
    printf("%s: %s", str, json_value_type_to_string(node->value.type));
    switch (node->value.type) {
        case JSON_VALUE_NUMBER:
            printf(", %.2f", node->value.num);
            break;
        case JSON_VALUE_STRING:
            printf(", %s", node->value.str);
            break;
        case JSON_VALUE_BOOLEAN:
            printf(", %s", node->value.b ? "true" : "false");
            break;
        default:
            break;
    }
    printf("\n");    
    for (size_t i = 0; i < node->child_count; i++) {
        if (strcmp(str, "/") != 0) {
            append_json_pointer(str, "/");
        }
        print_all_json_pointers_r(node->children[i], str);
        pop_json_pointer(str);
    }
}

void print_all_json_pointers(const json_tree_node *node) {
    char p[JSON_POINTER_PRINT_MAX_LENGTH];
    p[0] = '\0';
    print_all_json_pointers_r(node, p);
}

bool parse_json(const char *json, json_tree_node **root) {
    json_parser parser;
    bool success = init_json_parse(&parser, json) && parse_value(&parser, root, "/");
    if (parser.json != NULL) {
        free(parser.json);
        parser.json = NULL;
    }
    return success;
}

// validations
bool is_json_node_of_type(const json_tree_node *root, const char *json_pointer, json_value_type type) {
    const json_tree_node *node = get_json_node(root, json_pointer);
    if (node == NULL) {
        return false;
    }
    return node->value.type == type;
}

bool is_json_node_array_of_type(const json_tree_node *root, const char *json_pointer, json_value_type type) {
    const json_tree_node *node = get_json_node(root, json_pointer);
    if (node == NULL || node->value.type != JSON_VALUE_ARRAY) {
        return false;
    }
    for (size_t i = 0; i < node->child_count; i++) {
        if (node->children[i]->value.type != type) {
            return false;
        }
    }
    return true;
}