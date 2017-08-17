#include "surface.h"

#include <SDL2/SDL_syswm.h>
#include <string.h>
#include "../logger/logger.h"
#include "../vulkan_tools/vulkan_limits.h"
#include "../vulkan_tools/utils.h"
#include "../math/basic.h"

typedef bool (*create_surface_function)(const vk_functions *vk, VkSurfaceKHR*, 
    const vk_store*, SDL_Window*);

static bool select_desired_presentation_mode(const vk_functions *vk, vk_surface *surface, const vk_store *store,
    VkPresentModeKHR desired_present_mode) 
{
    uint32_t present_modes_count = 0; 
    VkPresentModeKHR present_modes[MAX_PRESENT_MODE_COUNT];
    bool result = get_available_present_modes(vk, store->physical_device, surface->surface_handle, 
        present_modes, &present_modes_count);

    if (!result) {
        return false; 
    }

    for (size_t i = 0; i < present_modes_count; i++) {
        if (present_modes[i] == desired_present_mode) {
            surface->present_mode = desired_present_mode;
            return true;
        }
    }

    debug_log("Desired present mode is not available, selecting the default");
    for (size_t i = 0; i < present_modes_count; i++) {
        if (present_modes[i] == VK_PRESENT_MODE_FIFO_KHR) {
            surface->present_mode = VK_PRESENT_MODE_FIFO_KHR;
            return true;
        }
    }

    error_log("Unable to select present mode");
    return false;
}

static bool init_swapchain_images(vk_surface *surface, uint32_t swapchain_images_count) {
    surface->swapchain_images_count = clamp(swapchain_images_count, 
        surface->capabilities.minImageCount, surface->capabilities.maxImageCount);
    return true;
}

static bool create_wayland_surface(const vk_functions *vk, VkSurfaceKHR *surface, 
    const vk_store *store, SDL_Window *w) 
{
    #if SDL_VERSION_ATLEAST(2, 0, 2)
        #if defined(VK_USE_PLATFORM_WAYLAND_KHR) && defined(SDL_VIDEO_DRIVER_WAYLAND)
            SDL_SysWMinfo info;
            SDL_VERSION(&info.version); 
            if (!SDL_GetWindowWMInfo(w, &info)) {
                error_log("SDL could not get WM info, SDL error: %s", SDL_GetError());
                return false;
            }
 
            VkWaylandSurfaceCreateInfoKHR surface_create_info =  {
                .sType   = VK_STRUCTURE_TYPE_WAYLAND_SURFACE_CREATE_INFO_KHR,
                .pNext   = NULL,
                .flags   = 0,
                .display = info.info.wl.display, 
                .surface = info.info.wl.surface
            };

            VkResult result = vk->CreateWaylandSurfaceKHR(
                store->instance,
                &surface_create_info,
                NULL, 
                surface
            );

            if (result != VK_SUCCESS || *surface == VK_NULL_HANDLE) {
                error_log("Could not create wayland surface");
                return false; 
            }

            debug_log("Successfully created wayland surface");
            return true;
        #else 
            error_log("Wayland platform not supported");
            return false;
        #endif
    #else 
        error_log("Update your SDL version");
        return false;
    #endif 
}

bool init_surface(const vk_functions *vk, vk_surface *surface, 
    const vk_store *store, SDL_Window *w) 
{
    surface->surface_handle = VK_NULL_HANDLE;
    surface->swapchain_images_count = 0;

    bool is_surface_extension = false; 
    create_surface_function f = NULL;
    for (size_t i = 0; i < store->loaded_extensions_count; i++) {
        if (strcmp(store->loaded_extensions[i], "VK_KHR_surface") == 0) {
            is_surface_extension = true;
        }
        if (strcmp(store->loaded_extensions[i], "VK_KHR_wayland_surface") == 0) {
            f = &create_wayland_surface;
        }
    }
    if (!is_surface_extension || f == NULL) {
        error_log("No surface extensions");
        return false;
    }

    return f(vk, &surface->surface_handle, store, w) &&
        select_desired_presentation_mode(vk, surface, store, VK_PRESENT_MODE_FIFO_KHR) &&
        get_surface_capabilities(vk, store->physical_device, surface->surface_handle, &surface->capabilities) && 
        init_swapchain_images(surface, 2);
}
