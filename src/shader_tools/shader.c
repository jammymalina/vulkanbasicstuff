#include "shader.h"

#include <stdlib.h>
#include "../utils/get_extension.h"
#include "../logger/logger.h"

static VkShaderStageFlags extension_to_shader_stage(const char *filename) {
    const char *extension = get_filename_ext(filename);
    if (strcmp(extension, "vert") == 0)
        return VK_SHADER_STAGE_VERTEX_BIT;
    if (strcmp(extension, "tesc") == 0)
        return VK_SHADER_STAGE_TESSELLATION_CONTROL_BIT;
    if (strcmp(extension, "tese") == 0)
        return VK_SHADER_STAGE_TESSELLATION_EVALUATION_BIT;
    if (strcmp(extension, "geom") == 0)
        return VK_SHADER_STAGE_GEOMETRY_BIT;
    if (strcmp(extension, "frag") == 0)
        return VK_SHADER_STAGE_FRAGMENT_BIT;
    if (strcmp(extension, "comp") == 0)
        return VK_SHADER_STAGE_COMPUTE_BIT;
    return VK_SHADER_STAGE_ALL;        
}

static bool read_binary_file(uint32_t buffer[MAX_SHADER_FILE_SIZE_BYTES], size_t *buffer_size, const char *filename) {
    FILE *file = NULL; 
    file = fopen(filename, "rb");

    *buffer_size = 0;

    if (!file) {
        error_log("Error while opening file: %s", filename);
        return false; 
    }

    fseek(file, 0, SEEK_END);
    *buffer_size = ftell(file);

    if (*buffer_size > MAX_SHADER_FILE_SIZE_BYTES) {
        error_log("Not enough space to read file: %s", filename);
        fclose(file);
        return false;
    }

    char byte_buffer[MAX_SHADER_FILE_SIZE_BYTES];

    fseek(file, 0, SEEK_SET);
    fread(byte_buffer, MAX_SHADER_FILE_SIZE_BYTES, 1, file);
    fclose(file);

    for (size_t i = 0; i < *buffer_size; i++) {
        buffer[i] = (uint32_t) byte_buffer[i];
    }

    return true;
}

void init_shader_module(vk_shader_module *shader_module) {
    shader_module->flags = 0;
    shader_module->handle = VK_NULL_HANDLE;
}

bool init_shader_module_from_filename(vk_shader_module *shader_module, const vk_functions *vk,
    const char *filename, VkShaderModuleCreateFlags flags, VkDevice device) 
{
    init_shader_module(shader_module);
    
    shader_module->flags = flags;

    uint32_t code_buffer[MAX_SHADER_FILE_SIZE_BYTES];
    size_t code_size = 0; 

    bool success = read_binary_file(code_buffer, &code_size, filename);

    if (!success) {
        return false;
    }

    VkShaderModuleCreateInfo shader_module_create_info = {
        .sType    = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO,
        .pNext    = NULL,
        .flags    = shader_module->flags,
        .codeSize = code_size,
        .pCode    = code_buffer
    };

    return vk->CreateShaderModule(device, &shader_module_create_info, NULL, &shader_module->handle) == VK_SUCCESS;
}

void destroy_shader_module(vk_shader_module *shader_module, const vk_functions *vk, VkDevice device) {
    if (shader_module != VK_NULL_HANDLE) {
        vk->DestroyShaderModule(device, shader_module->handle, NULL);        
    }
}
