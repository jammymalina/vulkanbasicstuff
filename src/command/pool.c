#include "pool.h"

void init_command_pool(command_pool *cp) {
    cp->handle = VK_NULL_HANDLE; 
}

bool init_command_pool_from_params(command_pool *cp, const vk_functions *vk, 
    VkDevice device, VkCommandPoolCreateFlags params, uint32_t queue_family_index)
{
    VkCommandPoolCreateInfo command_pool_create_info = {
        .sType            = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO,
        .pNext            = NULL,
        .flags            = params,
        .queueFamilyIndex = queue_family_index
    };
    return vk->CreateCommandPool(device, &command_pool_create_info, 
        NULL, &cp->handle) != VK_SUCCESS;
}

void destroy_command_pool(command_pool *cp, const vk_functions *vk, VkDevice device) {
    if (cp->handle != VK_NULL_HANDLE) {
        vk->DestroyCommandPool(device, cp->handle, NULL);
        cp->handle = VK_NULL_HANDLE;        
    }
}
