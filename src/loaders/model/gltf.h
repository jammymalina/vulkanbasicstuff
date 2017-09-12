#ifndef GLTF_MODEL_LOADER_H
#define GLTF_MODEL_LOADER_H

#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>
#include "../limits.h"

#define GLTF_HEADER_SIZE_BYTES 12
#define GLTF_MAGIC 0x46546C67
#define GLTF_VERSION 2
#define GLTF_CHUNK_TYPE_JSON 0x4E4F534A
#define GLTF_CHUNK_TYPE_BIN 0x004E4942

typedef struct {
    uint32_t magic;
    uint32_t version;
    uint32_t length;
} gltf_header;

typedef struct {
    uint32_t length; 
    uint32_t type;
    unsigned char *data;
} gltf_chunk;

bool load_model(const char *filename);

#endif // GLTF_MODEL_LOADER_H
