#ifndef VULKAN_FUNCTION_LOADER_H
#define VULKAN_FUNCTION_LOADER_H

#include <vulkan/vulkan.h>
#include <stdbool.h>
#include <stdint.h>
#include "functions.h"
#include "../vulkan_tools/vulkan_limits.h"

#if defined _WIN32 
	#include <windows.h> 
	#define LOAD_FUNCTION GetProcAddress 
	#define LIB_TYPE HINSTANCE
	#define FREE_LIB FreeLibrary
#elif defined __linux
	#include <dlfcn.h>
	#define LOAD_FUNCTION dlsym 
	#define LIB_TYPE void* 
	#define FREE_LIB dlclose 
#endif

bool connect_to_vulkan_library(LIB_TYPE *vulkan_lib);
bool load_basic_vulkan_functions(LIB_TYPE *vulkan_lib, vk_functions *vk); 
bool load_instance_vulkan_functions(vk_functions *vk, VkInstance instance, 
	const char loaded_extensions[MAX_VULKAN_EXTENSIONS][VK_MAX_EXTENSION_NAME_SIZE],
	uint32_t extensions_count);
bool load_device_level_functions(vk_functions *vk, VkDevice device, 
	const char loaded_extensions[MAX_VULKAN_EXTENSIONS][VK_MAX_EXTENSION_NAME_SIZE],
	uint32_t extensions_count);

#endif // VULKAN_FUNCTION_LOADER_H

