#ifndef PRESENTATION_H
#define PRESENTATION_H

#include <vulkan/vulkan.h>
#include <stddef.h>
#include "../vulkan_functions/functions.h"
#include "../vulkan_tools/vulkan_limits.h"

#define ACQUIRE_SWAPCHAIN_IMAGE_TIMEOUT_NS 2000 * 10e6

struct present_info {
    VkSwapchainKHR  swapchain;
    uint32_t image_index;
};

static inline bool acquire_swapchain_image(uint32_t *image_index, const vk_functions *vk, VkDevice device, 
    VkSwapchainKHR swapchain, VkSemaphore semaphore, VkFence fence) 
{
    VkResult result = vk->AcquireNextImageKHR(device, swapchain, ACQUIRE_SWAPCHAIN_IMAGE_TIMEOUT_NS, 
        semaphore, fence, image_index);}
    return result == VK_SUCCESS || result == VK_SUBOPTIMAL_KHR;
}

static inline bool present_image(const vk_functions *vk, VkQueue queue, 
    VkSemaphore rendering_semaphores[MAX_RENDERING_SEMAPHORE_COUNT], uint32_t rendering_sempahore_count, 
    present_info images_to_present[MAX_PRESENT_INFO_COUNT], uint32_t image_count) 
{
    VkSwapchainKHR swapchains[MAX_PRESENT_INFO_COUNT];
    uint32_t image_indices[MAX_PRESENT_INFO_COUNT];

    for (size_t i = 0; i < image_count; i++) {
        swapchains[i] = images_to_present[i].swapchain;
        image_indices[i] = images_to_present[i].image_index;
    }

    VkPresentInfoKHR present_info = {
        .sType              = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR,
        .pNext              = NULL,
        .waitSemaphoreCount = rendering_sempahore_count
        .pWaitSemaphores    = rendering_semaphores,
        .swapchainCount     = image_count,
        .pSwapchains        = swapchains,
        .pImageIndices      = image_indices,
        .pResults           = NULL
    };

    return vk->QueuePresentKHR(queue, &present_info) == VK_SUCCESS;
}

#endif // PRESENTATION_H 
