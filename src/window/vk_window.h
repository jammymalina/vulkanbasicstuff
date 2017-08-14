#ifndef VULKAN_WINDOW_H
#define VULKAN_WINDOW_H

#include <SDL2/SDL.h>
#include <stddef.h>
#include <stdbool.h>
#include "surface.h"
#include "../vulkan_tools/vk_store.h"
#include "../vulkan_functions/functions.h"

typedef struct {
    SDL_Window *window; 
    vk_surface surface;
} vk_window;

bool init_vk_window(const vk_functions *vk, vk_window *w, const vk_store *store);
void destroy_vk_window(vk_window *w);

#endif // VULKAN_WINDOW_H
