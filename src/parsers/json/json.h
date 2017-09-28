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

#define JSON_MAX_STRING_LENGTH 512
#define JSON_UNKNOWN_CHAR ((char) 129)

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
    JSON_VALUE_BOOLEAN,
    JSON_VALUE_NONE
} json_value_type;

typedef struct json_tree_node {
    struct {
        json_value_type type;
        union {
            double num;
            bool b;
            char str[JSON_MAX_STRING_LENGTH];
        } data;
    } value;

    char key[JSON_MAX_STRING_LENGTH];

    size_t child_count;
    size_t child_array_size;
    struct json_tree_node *parent;
    struct json_tree_node **children;
} json_tree_node;

bool parse_json(const char *json, json_tree_node **root);

void destroy_json_node(json_tree_node *node);

#endif // JSON_PARSER_H
