#include "trim.h"
#include <ctype.h>
#include <string.h>

void trim_left(char *str) {
    size_t len = strlen(str);
    char *cur = str;

    while (*cur && isspace(*cur)) {
        ++cur;
        --len;
    }

    if (str != cur) 
        memmove(str, cur, len + 1);
}

void trim_right(char *str) {
    size_t len = strlen(str);
    char *cur = str + len - 1;

    while (cur != str && isspace(*cur)) 
        --cur;
    cur[isspace(*cur) ? 0 : 1] = '\0';
}

void trim(char *str) {
    trim_right(str);
    trim_left(str);
}