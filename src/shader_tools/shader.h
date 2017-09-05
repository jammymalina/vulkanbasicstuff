#ifndef VULKAN_SHADER_H
#define VULKAN_SHADER_H

#include <vulkan/vulkan.h>
#include <stdint.h>
#include <stdbool.h>
#include "../vulkan_functions/functions.h"
#include "../vulkan_tools/vulkan_limits.h"

typedef struct {
    VkShaderModule handle;
    VkShaderModuleCreateFlags flags;
} vk_shader_module;

void init_shader_module(vk_shader_module *shader_module);
bool init_shader_module_from_filename(vk_shader_module *shader_module, const vk_functions *vk,
    const char *filename, VkShaderModuleCreateFlags flags, VkDevice device);
bool init_shader_modules(vk_shader_module shader_modules[MAX_SHADER_MODULE_COUNT], const char **filenames,
    uint32_t shader_module_count);
void destroy_shader_module(vk_shader_module *shader_module, const vk_functions *vk, VkDevice device);

#endif // VULKAN_SHADER_H
