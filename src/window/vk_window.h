#ifndef VULKAN_WINDOW_H
#define VULKAN_WINDOW_H

#include <SDL2/SDL.h>
#include <stddef.h>
#include <stdbool.h>
#include "surface.h"
#include "../vulkan_tools/vk_store.h"
#include "../vulkan_functions/functions.h"

#define WINDOW_WIDTH 1024 
#define WINDOW_HEIGHT 768

typedef struct {
    SDL_Window *window; 
    vk_surface surface;
} vk_window;

bool init_vk_window(vk_window *w, const vk_functions *vk, vk_store *store);
void destroy_vk_window(vk_window *w, const vk_functions *vk, vk_store *store);

#endif // VULKAN_WINDOW_H
