#include "gltf.h"

#include <string.h>
#include <stdlib.h>
#include "../json/json.h"
#include "../../utils/read_binary.h"
#include "../../logger/logger.h"

static bool get_gltf_header(gltf_header *header, byte_scanner *scanner);
static bool check_gltf_header(gltf_header *header);
static bool get_gltf_chunk(gltf_chunk *chunk, byte_scanner *scanner);
static bool check_json_chunk(const gltf_chunk *chunk);
static bool check_bin_chunk(const gltf_chunk *chunk);

static bool load_buffer_view(const json_tree_node *root, size_t buffer_view_index);

bool get_gltf_header(gltf_header *header, byte_scanner *scanner) {
    if (scanner->buffer_size < GLTF_HEADER_SIZE_BYTES) {
        return false;
    }
    header->magic = get_next_uint32_le(scanner);
    header->version = get_next_uint32_le(scanner);
    header->length = get_next_uint32_le(scanner);
    return true;
}

bool check_gltf_header(gltf_header *header) {
    return header->magic == GLTF_MAGIC && header->version == GLTF_VERSION;
}

bool get_gltf_chunk(gltf_chunk *chunk, byte_scanner *scanner) {
    chunk->length = 0;
    if (has_next_uint32(scanner)) {
        chunk->length = get_next_uint32_le(scanner);
        if (has_next_uint32(scanner)) {
            chunk->type = get_next_uint32_le(scanner);
            if (has_next_nbytes(scanner, chunk->length)) {
                chunk->data = malloc(chunk->length + 1);
                if (chunk->data != NULL) {
                    get_next_nbytes(scanner, chunk->data, chunk->length);
                    chunk->data[chunk->length] = '\0';
                    return true;
                }
            }
        }
    }
    return false;
}

bool check_json_chunk(const gltf_chunk *chunk) {
    return chunk->type == GLTF_CHUNK_TYPE_JSON;
}

bool check_bin_chunk(const gltf_chunk *chunk) {
    return chunk->type == GLTF_CHUNK_TYPE_BIN;
}

bool load_model(const char *filename) {
    unsigned char *buffer = malloc(MAX_MODEL_FILE_SIZE_BYTES);
    if (!buffer) {
        error_log("Could not allocate buffer for the file: %s", filename);
    }

    size_t buffer_size = 0;

    bool success = read_binary_file(buffer, &buffer_size, MAX_MODEL_FILE_SIZE_BYTES, filename);

    if (!success) {
        error_log("Error while reading model data: %s", filename);
        return false;
    }

    gltf_header header;
    gltf_chunk json_chunk, bin_chunk;

    byte_scanner scanner;
    init_byte_scanner(&scanner, buffer, buffer_size);

    json_tree_node *root;    

    success &= get_gltf_header(&header, &scanner) &&
        check_gltf_header(&header) &&
        get_gltf_chunk(&json_chunk, &scanner) &&
        check_json_chunk(&json_chunk) &&
        get_gltf_chunk(&bin_chunk, &scanner) &&
        check_bin_chunk(&bin_chunk);

    debug_log("json: %s", json_chunk.data);
    success = parse_json((const char*) json_chunk.data, &root);
    if (!success) {
        error_log("Error while reading model data: %s", filename);
        return false;
    }
    // print_all_json_pointers(get_json_node(root, "/bufferViews/2"));

    
    if (success) {
        free(buffer);
        free(json_chunk.data);
        free(bin_chunk.data);
    }

    return true;
}