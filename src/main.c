#include <stdio.h>
#include "vulkan_functions/functions.h"
#include "vulkan_functions/function_loader.h"
#include "vulkan_tools/vk_store.h"
#include "vulkan_tools/utils.h"
#include "vulkan_tools/string_utils.h"
#include "logger/logger.h"
#include "parsers/model/gltf.h"
#include "parsers/json/json.h"
#include "window/vk_window.h"

int main() {
	LIB_TYPE vulkan_lib = NULL;
	vk_functions vk;
	vk_store store;
	vk_window window;

	bool success = load_basic_vulkan_functions(&vulkan_lib, &vk) 
		&& init_store_from_config(&vk, &store, "./config/app.config")
		&& init_vk_window(&window, &vk, &store) 
		&& load_presentation_queues(&vk, &store.qb, store.physical_device, window.surface.surface_handle);

	if (!success) {
		error_log("Error during the intialization");
		goto exit_program;
	}

	load_model("resources/models/dragon/dragon.glb");

	SDL_Event e;
	bool is_finished = false;
	while (!is_finished){
		while (SDL_PollEvent(&e)){
			switch (e.type) {
				case SDL_QUIT:
					is_finished = true;
					break;
				case SDL_KEYUP:
					if (e.key.keysym.sym == SDLK_ESCAPE) {
						is_finished = true;
					}
					break;
			}
		}
	}
		
	exit_program:
	destroy_vk_window(&window, &vk, &store);	
	destroy_vulkan_store(&vk, &store);
	if (vulkan_lib != NULL) { 
		FREE_LIB(vulkan_lib);
	}
}
