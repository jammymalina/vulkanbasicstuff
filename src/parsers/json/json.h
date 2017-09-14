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

#define JSON_MAX_KEY_LENGTH 256

#include <stddef.h>
#include <stdbool.h>

typedef struct {
    char *json;
    size_t index;
} json_parser;

void init_json_parse(json_parser *parser, char *json);

#endif // JSON_PARSER_H
