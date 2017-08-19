#include "surface.h"

#ifdef VK_USE_PLATFORM_XCB_KHR
    #include <X11/Xlib-xcb.h>
#endif

#include <SDL2/SDL_syswm.h>
#include <string.h>
#include "vk_window.h"
#include "../logger/logger.h"
#include "../vulkan_tools/vulkan_limits.h"
#include "../vulkan_tools/utils.h"
#include "../vulkan_tools/swapchain_tools/utils.h"
#include "../math/basic.h"

typedef bool (*create_surface_function)(const vk_functions *vk, VkSurfaceKHR*, 
    vk_store*, SDL_Window*);

static bool select_desired_presentation_mode(vk_surface *surface, const vk_functions *vk, vk_store *store,
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

static bool create_swapchain(vk_surface *surface, const vk_functions *vk, vk_store *store) {
    VkSwapchainKHR old_swapchain = VK_NULL_HANDLE;

    VkSwapchainCreateInfoKHR swapchain_create_info = {
        .sType                 = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR,
        .pNext                 = NULL,
        .flags                 = 0,
        .surface               = surface->surface_handle,
        .minImageCount         = surface->swapchain_image_count,
        .imageFormat           = surface->image_format.format,
        .imageColorSpace       = surface->image_format.colorSpace,
        .imageExtent           = surface->image_size,
        .imageArrayLayers      = 1,
        .imageUsage            = surface->image_usage,
        .imageSharingMode      = VK_SHARING_MODE_EXCLUSIVE,
        .queueFamilyIndexCount = 0,
        .pQueueFamilyIndices   = NULL,
        .preTransform          = surface->transform,
        .compositeAlpha        = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR,
        .presentMode           = surface->present_mode,
        .clipped               = VK_TRUE,
        .oldSwapchain          = old_swapchain
    };

    VkResult result = vk->CreateSwapchainKHR(store->device, &swapchain_create_info, NULL, &surface->swapchain);
    if (result != VK_SUCCESS || surface->swapchain == VK_NULL_HANDLE) {
        error_log("Unable to create swapchain");
        return false;
    }

    if (old_swapchain != VK_NULL_HANDLE) {
        debug_log("Destroying old swapchain");
        vk->DestroySwapchainKHR(store->device, old_swapchain, NULL);
    }

    debug_log("Successfully created the swapchain");
    return true;
}

static bool init_swapchain(vk_surface *surface, const vk_functions *vk, vk_store *store, 
    VkImageUsageFlags image_usage) 
{
    surface->swapchain_image_count = surface->capabilities.minImageCount + 1;
    VkSurfaceFormatKHR available_formats[MAX_SURFACE_FORMAT_COUNT];
    uint32_t available_format_count = 0;
    VkSurfaceFormatKHR desired_format = { 
        .format     = VK_FORMAT_B8G8R8A8_UNORM, 
        .colorSpace = VK_COLOR_SPACE_SRGB_NONLINEAR_KHR 
    };
    set_swapchain_size(&surface->image_size, &surface->capabilities, WINDOW_WIDTH, WINDOW_HEIGHT);
    set_swapchain_transformation(&surface->transform, &surface->capabilities, 
        VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR);
    return set_swapchain_scenario(&surface->image_usage, &surface->capabilities, image_usage) &&
        get_available_surface_formats(vk, store->physical_device, surface->surface_handle, 
            available_formats, &available_format_count) &&
        set_swapchain_format(&surface->image_format, vk, store->physical_device, surface->surface_handle, 
            available_formats, available_format_count, desired_format) &&
        create_swapchain(surface, vk, store) && 
        get_swapchain_image_handles(vk, store->device, surface->swapchain, surface->swapchain_images, 
            &surface->swapchain_image_count);
}

static bool create_xcb_surface(const vk_functions *vk, VkSurfaceKHR *surface, 
    vk_store *store, SDL_Window *w) 
{
    *surface = VK_NULL_HANDLE;
        #if defined(VK_USE_PLATFORM_XCB_KHR) && defined(SDL_VIDEO_DRIVER_X11)
            SDL_SysWMinfo info;
            SDL_VERSION(&info.version); 
            if (!SDL_GetWindowWMInfo(w, &info)) {
                error_log("SDL could not get WM info, SDL error: %s", SDL_GetError());
                return false;
            }
 
            VkXcbSurfaceCreateInfoKHR surface_create_info =  {
                .sType      = VK_STRUCTURE_TYPE_XCB_SURFACE_CREATE_INFO_KHR,
                .pNext      = NULL,
                .flags      = 0,
                .connection = XGetXCBConnection(info.info.x11.display), 
                .window     = info.info.x11.window
            };

            VkResult result = vk->CreateXcbSurfaceKHR(
                store->instance,
                &surface_create_info,
                NULL, 
                surface
            );

            if (result != VK_SUCCESS || *surface == VK_NULL_HANDLE) {
                error_log("Could not create xcb surface");
                return false; 
            }

            debug_log("Successfully created xcb surface");
            return true;
    #else 
        error_log("xcb platform not supported");
        return false;
    #endif 
}

static bool create_wayland_surface(const vk_functions *vk, VkSurfaceKHR *surface, 
    vk_store *store, SDL_Window *w) 
{
    *surface = VK_NULL_HANDLE;
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

void init_surface(vk_surface *surface) {
    surface->surface_handle         = VK_NULL_HANDLE;
    surface->swapchain_image_count = 0;
    surface->swapchain              = VK_NULL_HANDLE;
}

bool init_surface_from_window(vk_surface *surface, const vk_functions *vk, 
    vk_store *store, SDL_Window *w) 
{
    init_surface(surface);

    bool is_surface_extension = false; 
    create_surface_function f = NULL;
    for (size_t i = 0; i < store->loaded_extensions_count; i++) {
        if (strcmp(store->loaded_extensions[i], "VK_KHR_surface") == 0) {
            is_surface_extension = true;
        } else if (strcmp(store->loaded_extensions[i], "VK_KHR_wayland_surface") == 0) {
            f = &create_wayland_surface;
        } else if (strcmp(store->loaded_extensions[i], "VK_KHR_xcb_surface") == 0) {
            f = &create_xcb_surface;
        }
    }
    if (!is_surface_extension || f == NULL) {
        error_log("No surface extensions");
        return false;
    }

    return f(vk, &surface->surface_handle, store, w) &&
        select_desired_presentation_mode(surface, vk, store, VK_PRESENT_MODE_FIFO_KHR) &&
        get_surface_capabilities(vk, store->physical_device, surface->surface_handle, &surface->capabilities) && 
        init_swapchain(surface, vk, store, VK_IMAGE_ASPECT_COLOR_BIT | VK_IMAGE_ASPECT_DEPTH_BIT);
}

void destroy_surface(vk_surface *surface, const vk_functions *vk, vk_store *store) {
    if(surface->swapchain != VK_NULL_HANDLE) {
        vk->DestroySwapchainKHR(store->device, surface->swapchain, NULL);
        surface->swapchain = VK_NULL_HANDLE;
    }
    if(surface->surface_handle != VK_NULL_HANDLE) {
        vk->DestroySurfaceKHR(store->instance, surface->surface_handle, NULL);
        surface->surface_handle = VK_NULL_HANDLE;
    }
}
