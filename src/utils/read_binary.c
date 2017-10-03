#include "read_binary.h"

#include <string.h>

size_t get_file_size(FILE *file) {
    if (!file) {
        return 0; 
    }

    size_t buffer_size = 0;    
    size_t current_pos = ftell(file);

    fseek(file, 0, SEEK_END);
    buffer_size = ftell(file);
    fseek(file, 0, current_pos);

    return buffer_size;
}

bool read_binary_file(unsigned char *buffer, size_t *buffer_size, size_t max_buffer_size, const char *filename) {
    FILE *file = NULL; 
    file = fopen(filename, "rb");

    *buffer_size = 0;

    if (!file) {
        error_log("Error while opening file: %s", filename);
        return false; 
    }

    *buffer_size = get_file_size(file);

    if (*buffer_size > max_buffer_size) {
        error_log("Not enough space to read file: %s", filename);
        fclose(file);
        return false;
    }

    fseek(file, 0, SEEK_SET);
    fread(buffer, *buffer_size, 1, file);
    fclose(file);

    return true;
}

void init_byte_scanner(byte_scanner *scanner, unsigned char *buffer, size_t buffer_size) {
    scanner->index = 0;
    scanner->buffer = buffer; 
    scanner->buffer_size = buffer_size;
}

bool has_next_nbytes(byte_scanner *scanner, size_t n) {
    return (scanner->index + n) <= scanner->buffer_size;
}

bool has_next_uint32(byte_scanner *scanner) {
    return has_next_nbytes(scanner, sizeof(uint32_t));
}

void get_next_string(byte_scanner *scanner, char *str, size_t n) {
    for (size_t i = 0; i < n; i++) {
        str[i] = scanner->buffer[scanner->index + i];
    }
    str[n] = '\0';
    scanner->index += n;
}

void get_next_nbytes(byte_scanner *scanner, unsigned char *bytes, size_t n) {
    memcpy(bytes, scanner->buffer + scanner->index, n);
    scanner->index += n;
}

uint32_t get_next_uint32_le(byte_scanner *scanner) {
    uint32_t result = scanner->buffer[scanner->index + 0] +
        (scanner->buffer[scanner->index + 1] << 8) +
        (scanner->buffer[scanner->index + 2] << 16) +
        (scanner->buffer[scanner->index + 3] << 24);
    scanner->index += sizeof(uint32_t);
    return result;
}

uint32_t get_next_uint32_be(byte_scanner *scanner) {
    uint32_t result = (scanner->buffer[scanner->index + 0] << 24) +
        (scanner->buffer[scanner->index + 1] << 16) +
        (scanner->buffer[scanner->index + 2] << 8) +
        scanner->buffer[scanner->index + 3];    
    scanner->index += sizeof(uint32_t);
    return result;       
}

