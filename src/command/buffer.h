#ifndef COMMAND_BUFFER_H
#define COMMAND_BUFFER_H

#include <vulkan/vulkan.h>
#include <stddef.h> 
#include <stdbool.h>
#include <stdint.h>
#include "pool.h"
#include "../vulkan_functions/functions.h"
#include "../vulkan_tools/vulkan_limits.h"

typedef struct {
    VkCommandBuffer buffers[MAX_COMMAND_BUFFER_COUNT];
    uint32_t buffer_count;

    VkCommandBufferLevel level;
} command_buffer;

void init_command_buffer(command_buffer *buf); 
bool allocate_command_buffer(command_buffer *buf, const vk_functions *vk, VkDevice device, 
    VkCommandPool pool, VkCommandBufferLevel level, uint32_t buffer_count);
void destroy_command_buffer(command_buffer *buf, const vk_functions *vk, VkDevice device, VkCommandPool pool);

bool begin_command_buffer_recording_operation(command_buffer *buf, uint32_t buffer_index, const vk_functions *vk, 
    VkCommandBufferUsageFlags usage, VkCommandBufferInheritanceInfo *secondary_command_buffer_info);
bool end_command_buffer_recording_operation(command_buffer *buf, uint32_t buffer_index, const vk_functions *vk);
bool reset_command_buffer(command_buffer *buf, uint32_t buffer_index, const vk_functions *vk, bool release_resources);
        

#endif // COMMAND_BUFFER_H
