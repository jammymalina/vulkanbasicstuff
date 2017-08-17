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
    uint32_t swapchain_images_count;
} vk_surface;

bool init_surface(const vk_functions *vk, vk_surface *surface, 
    const vk_store *store, SDL_Window *w);
bool destroy_surface(vk_surface *surface);

#endif // VULKAN_SURFACE_H
