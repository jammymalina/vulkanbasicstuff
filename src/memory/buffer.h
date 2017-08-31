#ifndef VULKAN_MEMORY_BUFFER_H
#define VULKAN_MEMORY_BUFFER_H

#include <vulkan/vulkan.h>
#include <stdbool.h>
#include "../vulkan_functions/functions.h"

typedef struct {
    VkBuffer handle;
    VkBufferUsageFlags usage;
    VkDeviceSize size;
} vk_memory_buffer;

void init_vk_memory_buffer(vk_memory_buffer *buf);
bool init_vk_memory_buffer_from_params(vk_memory_buffer *buf, const vk_functions *vk, VkDevice device, 
    VkBufferUsageFlags usage, VkDeviceSize size);

#endif // VULKAN_MEMORY_BUFFER_H
