#ifndef VULKAN_SYNC_H
#define VULKAN_SYNC_H

#include <vulkan/vulkan.h>
#include <stdbool.h>
#include <stdint.h>
#include "../vulkan_functions/functions.h"
#include "../vulkan_tools/vulkan_limits.h"

typedef struct {
    VkSemaphore handle;
    VkPipelineStageFlags waiting_stage;
} vk_semaphore;

typedef struct {
    VkFence handle;
    bool signaled;
} vk_fence;

bool init_semaphore(vk_semaphore *semaphore, const vk_functions *vk, VkDevice device);
bool init_semaphore_waiting_stage(vk_semaphore *semaphore, const vk_functions *vk, VkDevice device, 
    VkPipelineStageFlags waiting_stage);

bool init_fence(vk_fence *fence, const vk_functions *vk, VkDevice device, bool signaled);
bool wait_for_fences(const vk_fence *fences, uint32_t fence_count, const vk_functions *vk, VkDevice device, 
    VkBool32 wait_for_all, uint64_t timeout);
bool reset_fences(const vk_fence *fences, uint32_t fence_count, const vk_functions *vk, VkDevice device);

#endif // VULKAN_SYNC_H
