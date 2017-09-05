#include "get_extension.h"
#include "strcatcpy.h"

const char *get_filename_ext(const char *filename) {
    const char *dot = strrchr(filename, '.');
    if(!dot || dot == filename) {
        return "";
    }
    return dot + 1;
}

const char *get_filename_extn(const char *filename, int index) {
    size_t i = 0;
    size_t dot_start_index = 0, dot_end_index = 0;
    size_t ext_len = dot_end_index - dot_start_index;
    const char *result = (const char*) malloc(ext_len + 1);
    if (!result) {
        return "";
    }

    return result;
}