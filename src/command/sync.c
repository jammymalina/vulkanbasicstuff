#include "sync.h"

#include <stddef.h>

bool init_semaphore(vk_semaphore *semaphore, const vk_functions *vk, VkDevice device) {
    return init_semaphore_waiting_stage(semaphore, vk, device, 0);        
}

bool init_semaphore_waiting_stage(vk_semaphore *semaphore, const vk_functions *vk, VkDevice device, 
    VkPipelineStageFlags waiting_stage) 
{
    semaphore->handle = VK_NULL_HANDLE; 
    semaphore->waiting_stage = waiting_stage;
    VkSemaphoreCreateInfo semaphore_create_info = {
        .sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO,
        .pNext = NULL, 
        .flags = 0
    };
    return vk->CreateSemaphore(device, &semaphore_create_info, NULL, &semaphore->handle) == VK_SUCCESS;
}

bool init_fence(vk_fence *fence, const vk_functions *vk, VkDevice device, bool signaled) {
    fence->handle = VK_NULL_HANDLE;
    fence->signaled = signaled;
    VkFenceCreateInfo fence_create_info = {
        .sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO,
        .pNext = NULL,
        .flags = fence->signaled ? VK_FENCE_CREATE_SIGNALED_BIT : 0
    };
    return vk->CreateFence(device, &fence_create_info, NULL, &fence->handle) == VK_SUCCESS;
}

bool wait_for_fences(const vk_fence fences[MAX_FENCE_COUNT], uint32_t fence_count, const vk_functions *vk, 
    VkDevice device, VkBool32 wait_for_all, uint64_t timeout) 
{
    if (fence_count <= 0) {
        return false; 
    } 
    VkFence fence_handles[MAX_FENCE_COUNT];
    for (size_t i = 0; i < fence_count; i++) {
        fence_handles[i] = fences[i].handle;
    }
    return vk->WaitForFences(device, fence_count, fence_handles, wait_for_all, timeout) == VK_SUCCESS;
}

bool reset_fences(const vk_fence fences[MAX_FENCE_COUNT], uint32_t fence_count, const vk_functions *vk, VkDevice device) {
    if (fence_count <= 0) {
        return false; 
    } 
    VkFence fence_handles[MAX_FENCE_COUNT];
    for (size_t i = 0; i < fence_count; i++) {
        fence_handles[i] = fences[i].handle;
    }
    return vk->ResetFences(device, fence_count, fence_handles);
}