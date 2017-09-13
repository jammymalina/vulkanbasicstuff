#ifndef GLTF_MODEL_LOADER_H
#define GLTF_MODEL_LOADER_H

#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>
#include "../limits.h"

// from spec
#define GLTF_HEADER_SIZE_BYTES 12
#define GLTF_MAGIC 0x46546C67 // glTF
#define GLTF_VERSION 2
#define GLTF_CHUNK_TYPE_JSON 0x4E4F534A
#define GLTF_CHUNK_TYPE_BIN 0x004E4942

// custom stuff
#define GLTF_MAX_ACCESSOR_COUNT 32
#define GLTF_MAX_BUFFER_COUNT 32
#define GLTF_MAX_BUFFER_VIEW_COUNT 32
#define GLTF_MAX_IMAGE_COUNT 32
#define GLTF_MAX_MATERIAL_COUNT 32
#define GLTF_MAX_MESH_COUNT 16
#define GLTF_MAX_NODE_COUNT 64
#define GLTF_MAX_SAMPLER_COUNT 32
#define GLTF_MAX_TEXTURE_COUNT 32

#define GLTF_MAX_NAME_LENGTH 256
#define GLTF_MAX_URI_LENGTH 512

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

typedef enum {
    GLTF_BYTE = 5120,
    GLTF_UNSIGNED_BYTE = 5121,
    GLTF_SHORT = 5122,
    GLTF_UNSIGNED_SHORT = 5123,
    GLTF_UNSIGNED_INT = 5125,
    GLTF_FLOAT = 5126
} gltf_component_type;

typedef enum {
    GLTF_ARRAY_BUFFER = 34962,
    GLTF_ELEMENT_ARRAY_BUFFER = 34963
} gltf_target;

typedef float gltf_type[16];

typedef struct {
    uint32_t buffer_view;
    uint32_t byte_offset;
    gltf_component_type component_type;
    bool normalized;
    uint32_t count;
    char type[7];
    gltf_type min;
    gltf_type max;
    char name[GLTF_MAX_NAME_LENGTH];
} gltf_accessor;

typedef struct {
    char uri[GLTF_MAX_URI_LENGTH];
    uint32_t byte_length;
    char name[GLTF_MAX_NAME_LENGTH];
} gtlf_buffer;

typedef struct {
    uint32_t buffer;
    uint32_t byte_offset;
    uint32_t byte_length;
    uint8_t byte_stride;
    gltf_target target;
    char name[GLTF_MAX_NAME_LENGTH];
} gltf_buffer_view;

typedef struct {
    char uri[GLTF_MAX_URI_LENGTH];
    char mime_type[11];
    uint32_t buffer_view;
    char name[GLTF_MAX_NAME_LENGTH];
} gltf_image;

typedef {
    float base_color_factor[4];
    
} gltf_pbr_metallic_roughness;

typedef struct {
    char name[GLTF_MAX_NAME_LENGTH];
    float emissive_factor[3];
    char alpha_mode[7];
    float alpha_cutoff;
    bool double_sided;
} gltf_material;

bool load_model(const char *filename);

#endif // GLTF_MODEL_LOADER_H
