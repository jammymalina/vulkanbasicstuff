#ifndef READ_BINARY_FILE_H
#define READ_BINARY_FILE_H

#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>
#include "../logger/logger.h"

bool read_binary_file(unsigned char *buffer, size_t *buffer_size, size_t max_buffer_size, const char *filename);

typedef struct {
    size_t index;
    unsigned char *buffer;
    size_t buffer_size;
} byte_scanner;

void init_byte_scanner(byte_scanner *scanner, unsigned char *buffer, size_t buffer_size);
bool has_next_nbytes(byte_scanner *scanner, size_t n);
bool has_next_uint32(byte_scanner *scanner);
void get_next_string(byte_scanner *scanner, char *str, size_t n);
void get_next_nbytes(byte_scanner *scanner, unsigned char *bytes, size_t n);
uint32_t get_next_uint32_le(byte_scanner *scanner); // little-endian
uint32_t get_next_uint32_be(byte_scanner *scanner); // big-endian

#endif // READ_BINARY_FILE_H
