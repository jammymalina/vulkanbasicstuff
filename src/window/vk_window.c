#include "vk_window.h"

#include "../logger/logger.h"

bool init_vk_window(const vk_functions *vk, vk_window *w, const vk_store *store) {
    w->window         = NULL; 

    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        error_log("SDL could not initialize, SDL error: %s", SDL_GetError());
        return false;
    }

    w->window = SDL_CreateWindow("SDL Vulkan tutorial", 
        SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 600, 480, SDL_WINDOW_SHOWN);
    if (w->window == NULL) {
        error_log("SDL could not create window, SDL error: %s", SDL_GetError());
        return false;
    }
    bool success = init_surface(vk, &w->surface, store, w->window);
    if (!success) {
        error_log("Couldn't create surface");
        return false;
    }

    //Wait two seconds
    SDL_Delay(2000);

    return true;
}

void destroy_vk_window(vk_window *w) {
    if (w->window != NULL) {
        SDL_DestroyWindow(w->window);
        w->window = NULL;
    }
    SDL_Quit();
}