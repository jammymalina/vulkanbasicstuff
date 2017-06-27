#include <stdio.h>
#include "vulkan_functions/functions.h"
#include "vulkan_functions/function_loader.h"
#include "vulkan_tools/vk_store.h"
#include "logger/logger.h"

int main() {
	LIB_TYPE vulkan_lib = NULL;
	vk_functions vk;
	vk_store store;

	bool success = load_all_vulkan_functions(&vulkan_lib, &vk) 
		&& load_extensions(&vk, &store)
		&& create_instance(&vk, &store, "./config/app.config");
	
	debug_log("Supported extensions: "); 
	for (size_t i = 0; i < store.available_extensions_count; i++) {
		debug_log("  %s, version: %d", store.available_extensions[i].extensionName, 
				store.available_extensions[i].specVersion);
	}

	if (!success) {
		goto exit_program;
	}
	
	exit_program:
	if (vulkan_lib != NULL) { 
		FREE_LIB(vulkan_lib);
	}
}
