#include "json.h"

#include <string.h>
#include <ctype.h>

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

void init_json_parse(json_parser *parser, char *json) {
    parser->index = 0; 
    parser->json = json;
}


