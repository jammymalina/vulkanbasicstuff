#ifndef JSON_PARSER_H 
#define JSON_PARSER_H

#define JSON_TOKEN_NONE 0
#define JSON_TOKEN_CURLY_OPEN 1 
#define JSON_TOKEN_CURLY_CLOSE 2
#define JSON_TOKEN_SQUARED_OPEN 3 
#define JSON_TOKEN_SQUARED_CLOSE 4 
#define JSON_TOKEN_COLON 5 
#define JSON_TOKEN_COMMA 6
#define JSON_TOKEN_STRING 7 
#define JSON_TOKEN_NUMBER 8
#define JSON_TOKEN_TRUE 9
#define JSON_TOKEN_FALSE 10
#define JSON_TOKEN_NULL 11

#define JSON_MAX_KEY_LENGTH 512
#define JSON_MAX_STRING_LENGTH 512

#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>
#include <string.h>

typedef enum {
    JSON_VALUE_NUMBER,
    JSON_VALUE_ARRAY,
    JSON_VALUE_OBJECT,
    JSON_VALUE_STRING,
    JSON_VALUE_NULL,
    JSON_VALUE_BOOLEAN
} json_value_type;

typedef struct json_tree_node {
    struct {
        json_value_type type;
        union {
            int i;
            bool b;
            char str[JSON_MAX_STRING_LENGTH];
        } data;
    } value;

    char key[JSON_MAX_KEY_LENGTH];

    size_t child_count;
    size_t child_array_size;
    struct json_tree_node *parent;
    struct json_tree_node **children;
} json_tree_node;

typedef struct {
    char *json;
    size_t index;
} json_parser;

void init_json_parse(json_parser *parser, char *json);
bool parse_json(json_parser *parser, json_tree_node **root);

#endif // JSON_PARSER_H
