#ifndef VULKAN_SURFACE_H 
#define VULKAN_SURFACE_H

#include <vulkan/vulkan.h>
#include <SDL2/SDL.h>
#include "../vulkan_tools/vk_store.h"
#include "../vulkan_functions/functions.h"

typedef struct {
    VkSurfaceKHR surface_handle;

    VkPresentModeKHR present_mode;
    VkSurfaceCapabilitiesKHR capabilities;
    VkSurfaceTransformFlagBitsKHR transform;
    VkExtent2D image_size;
    VkImageUsageFlags image_usage;
    VkSurfaceFormatKHR image_format;
    
    VkSwapchainKHR swapchain;
    VkImage swapchain_images[MAX_SWAPCHAIN_IMAGE_COUNT];
    VkImageView swapchain_image_views[MAX_SWAPCHAIN_IMAGE_COUNT];
    uint32_t swapchain_image_count;
    uint32_t swapchain_image_view_count; // should be equal to swapchain_image_count
    VkComponentMapping component_mapping;
    VkImageSubresourceRange subresource_range;
} vk_surface;

void init_surface(vk_surface *surface);
bool init_surface_from_window(vk_surface *surface, const vk_functions *vk, 
    vk_store *store, SDL_Window *w);
void destroy_surface(vk_surface *surface, const vk_functions *vk, vk_store *store);

#endif // VULKAN_SURFACE_H
