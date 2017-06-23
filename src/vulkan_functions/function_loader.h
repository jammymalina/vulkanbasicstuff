#ifndef VULKAN_FUNCTION_LOADER_H
#define VULKAN_FUNCTION_LOADER_H

#include <stdbool.h>
#include "functions.h"

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
bool load_all_vulkan_functions(LIB_TYPE *vulkan_lib, vk_functions *vk); 

#endif // VULKAN_FUNCTION_LOADER_H

