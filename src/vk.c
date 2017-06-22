#include "vk.h"

vk_functions vk;

LIB_TYPE vulkan_lib = NULL; 
bool success = connect_to_vulkan_library(vulkan_lib) && load_all_vulkan_functions(vulkan_lib, &vk);

bool destroy_vk() {
	if (vulkan_lib != NULL) {
		FREE_LIB(vulkan_lib);
	}
}

