#include "buffer.h"

void init_command_buffer(command_buffer *buf) {
    buf->buffer_count = 0;
}

bool allocate_command_buffer(command_buffer *buf, const vk_functions *vk, VkDevice device, 
    command_pool *pool, VkCommandBufferLevel level, uint32_t buffer_count) 
{
    destroy_command_buffer(buf, vk, device, pool);
    buf->level = level;
    VkCommandBufferAllocateInfo command_buffer_allocate_info = {
        .sType              = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO,
        .pNext              = NULL,
        .commandPool        = pool->handle,
        .level              = buf->level,
        .commandBufferCount = buffer_count
    };
    bool success = vk->AllocateCommandBuffers(device, &command_buffer_allocate_info, buf->buffers) == VK_SUCCESS;
    if (success) {
        buf->buffer_count = buffer_count;
    }
    return success;
}

void destroy_command_buffer(command_buffer *buf, const vk_functions *vk, VkDevice device, command_pool *pool) {
    if(buf->buffer_count > 0) {
        vk->FreeCommandBuffers(device, pool->handle, buf->buffer_count, buf->buffers);
        buf->buffer_count = 0;
    }
}

bool begin_command_buffer_recording_operation(command_buffer *buf, uint32_t buffer_index, const vk_functions *vk, 
    VkCommandBufferUsageFlags usage, VkCommandBufferInheritanceInfo *secondary_command_buffer_info)
{
    if (buffer_index >= buf->buffer_count) {
        return false;
    }
    VkCommandBufferBeginInfo command_buffer_begin_info = {
        .sType            = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO,
        .pNext            = NULL,
        .flags            = usage,
        .pInheritanceInfo = secondary_command_buffer_info
    };
    return vk->BeginCommandBuffer(buf->buffers[buffer_index], &command_buffer_begin_info) == VK_SUCCESS;
}

bool end_command_buffer_recording_operation(command_buffer *buf, uint32_t buffer_index, const vk_functions *vk) {
    if (buffer_index >= buf->buffer_count) {
        return false; 
    }
    return vk->EndCommandBuffer(buf->buffers[buffer_index]) == VK_SUCCESS;
}

bool reset_command_buffer(command_buffer *buf, uint32_t buffer_index, const vk_functions *vk, bool release_resources) {
    if (buffer_index >= buf->buffer_count) {
        return false; 
    }
    return vk->ResetCommandBuffer(buf->buffers[buffer_index], release_resources ? 
        VK_COMMAND_BUFFER_RESET_RELEASE_RESOURCES_BIT : 0) == VK_SUCCESS;
}

