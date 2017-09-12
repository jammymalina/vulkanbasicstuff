#include "get_extension.h"
#include "strcatcpy.h"

const char *get_filename_ext(const char *filename) {
    const char *dot = strrchr(filename, '.');
    if(!dot || dot == filename) {
        return "";
    }
    return dot + 1;
}