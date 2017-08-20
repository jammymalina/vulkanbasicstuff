#ifndef COMMAND_POOL_H 
#define COMMAND_POOL_H

#include <vulkan/vulkan.h>
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include "../vulkan_functions/functions.h"

typedef struct {
    VkCommandPool handle;
    VkCommandPoolCreateFlags parameters;
} command_pool;

void init_command_pool(command_pool *cp);
bool init_command_pool_from_params(command_pool *cp, const vk_functions *vk, 
    VkDevice device, VkCommandPoolCreateFlags params, uint32_t queue_family_index);
void destroy_command_pool(command_pool *cp, const vk_functions *vk, VkDevice device);

#endif // COMMAND_POOL_H
