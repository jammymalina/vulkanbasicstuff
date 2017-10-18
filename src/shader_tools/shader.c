#include "shader.h"

#include <stdlib.h>
#include "../utils/read_binary.h"
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

void init_shader_module(vk_shader_module *shader_module) {
    shader_module->flags = 0;
    shader_module->handle = VK_NULL_HANDLE;
}

bool init_shader_module_from_filename(vk_shader_module *shader_module, const vk_functions *vk,
    const char *filename, VkShaderModuleCreateFlags flags, VkDevice device) 
{
    init_shader_module(shader_module);
    
    shader_module->flags = flags;

    unsigned char byte_buffer[MAX_SHADER_FILE_SIZE_BYTES];
    uint32_t code_buffer[MAX_SHADER_FILE_SIZE_BYTES];
    size_t code_size = 0; 

    bool success = read_binary_file(byte_buffer, &code_size, MAX_SHADER_FILE_SIZE_BYTES, filename);

    if (!success) {
        return false;
    }

    for (size_t i = 0; i < code_size; i++) {
        code_buffer[i] = (uint32_t) byte_buffer[i];
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

bool init_shader_stage_info(VkPipelineShaderStageCreateInfo *shader_stage, const vk_functions *vk, const char *filename) {
    shader_stage->sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    vertShaderStageInfo->stage = extension_to_shader_stage(filename);
        
    return true;
}
