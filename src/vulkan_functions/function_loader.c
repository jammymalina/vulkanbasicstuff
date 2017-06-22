#include "function_loader.h"

#include <stddef.h>
#include <stdio.h>
#include <stdbool.h> 

static bool load_exported_vulkan_function(LIB_TYPE vulkan_lib, vk_functions *vk); 
static bool load_global_functions(vk_functions *vk);

bool connect_to_vulkan_library(LIB_TYPE vulkan_lib) {
	vulkan_lib = NULL;
	#if defined _WIN32
		vulkan_lib = LoadLibrary("vulkan-1.dll"); 
	#elif defined __linux 
		vulkan_lib = dlopen("libvulkan.so.1", RTLD_NOW); 
	#endif

	if (vulkan_lib == NULL) {
		printf("Could not connect with a Vulkan runtime library.\n"); 
		return false; 
	}

	return true;
}

bool load_all_vulkan_functions(LIB_TYPE vulkan_lib, vk_functions *vk) {
	bool success = connect_to_vulkan_library(vulkan_lib)
		&& load_global_functions(vk);
		
	return success;
}

bool load_exported_vulkan_function(LIB_TYPE vulkan_lib, vk_functions *vk) {
	#if defined _WIN32
		#define LOAD_FUNCTION GetProcAddress 
	#elif defined __linux
		#define LOAD_FUNCTION dlsym  	
	#endif

	#define EXPORTED_VULKAN_FUNCTION(name)                                     \
		vk->name = (PFN_vk##name) LOAD_FUNCTION(vulkan_lib, "vk" #name);       \
		if (vk->name == NULL) {                                                \
			printf("Could not load exported Vulkan function named: vk" #name); \
			return false;                                                      \
		}

	#include "list.inl"

	return true;
}

bool load_global_functions(vk_functions *vk) {
	#define GLOBAL_LEVEL_VULKAN_FUNCTION(name)                               \
		vk->name = (PFN_vk##name) vk->GetInstanceProcAddr(NULL, "vk" #name); \
		if(vk->name == NULL) {                                               \
			printf("Could not load global level function: vk" #name);        \
			return false;                                                    \
		}

	#include "list.inl"

	return true;
}

