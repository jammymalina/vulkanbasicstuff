#include "vk_window.h"

#include "../logger/logger.h"

bool init_vk_window(vk_window *w, const vk_functions *vk, vk_store *store) {
    w->window = NULL; 

    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        error_log("SDL unable to initialize, SDL error: %s", SDL_GetError());
        return false;
    }

    w->window = SDL_CreateWindow("SDL Vulkan tutorial", 
        SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
    if (w->window == NULL) {
        error_log("SDL unable to create window, SDL error: %s", SDL_GetError());
        return false;
    }
    bool success = init_surface_from_window(&w->surface, vk, store, w->window);
    if (!success) {
        error_log("Unable to create surface");
        return false;
    }

    //Wait two seconds
    SDL_Delay(2000);

    return true;
}

void destroy_vk_window(vk_window *w, const vk_functions *vk, vk_store *store) {
    destroy_surface(&w->surface, vk, store);
    if (w->window != NULL) {
        SDL_DestroyWindow(w->window);
        w->window = NULL;
    }
    SDL_Quit();
}