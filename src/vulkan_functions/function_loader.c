#include "function_loader.h"

#include <string.h>
#include <stddef.h>
#include <stdbool.h>
#include "../logger/logger.h"

static bool load_exported_vulkan_function(LIB_TYPE vulkan_lib, vk_functions *vk); 
static bool load_global_functions(vk_functions *vk);

bool connect_to_vulkan_library(LIB_TYPE *vulkan_lib) {
	*vulkan_lib = NULL;
	#if defined _WIN32
		*vulkan_lib = LoadLibrary("vulkan-1.dll"); 
	#elif defined __linux 
		*vulkan_lib = dlopen("libvulkan.so.1", RTLD_NOW); 
	#endif

	if (*vulkan_lib == NULL) {
		error_log("Could not connect with a Vulkan runtime library."); 
		return false; 
	} else {
		debug_log("Successfully connected to Vulkan library.");
	}

	return true;
}

bool load_basic_vulkan_functions(LIB_TYPE *vulkan_lib, vk_functions *vk) {
	bool success = connect_to_vulkan_library(vulkan_lib)
		&& load_exported_vulkan_function(*vulkan_lib, vk) 
		&& load_global_functions(vk);
		
	return success;
}

bool load_exported_vulkan_function(LIB_TYPE vulkan_lib, vk_functions *vk) {
	#define EXPORTED_VULKAN_FUNCTION(name)                                               \
		vk->name = (PFN_vk##name) LOAD_FUNCTION(vulkan_lib, "vk" #name);                 \
		if (vk->name == NULL) {                                                          \
			error_log("Could not load exported Vulkan function: vk" #name);              \
			return false;                                                                \
		} else {                                                                         \
			debug_log("Successfully loaded exported Vulkan function: vk" #name);         \
		}

	#include "list.inl"

	return true;
}

bool load_global_functions(vk_functions *vk) {
	#define GLOBAL_LEVEL_VULKAN_FUNCTION(name)                                           \
		vk->name = (PFN_vk##name) vk->GetInstanceProcAddr(NULL, "vk" #name);             \
		if(vk->name == NULL) {                                                           \
			error_log("Could not load global level function: vk" #name);                 \
			return false;                                                                \
		} else {                                                                         \
			debug_log("Successfully loaded global Vulkan function: vk" #name);           \
		}

	#include "list.inl"

	return true;
}

bool load_instance_vulkan_functions(vk_functions *vk, VkInstance instance,
	const char loaded_extensions[MAX_VULKAN_EXTENSIONS][VK_MAX_EXTENSION_NAME_SIZE],
	uint32_t extensions_count)
{
	#define INSTANCE_LEVEL_VULKAN_FUNCTION(name)                                         \
		vk->name = (PFN_vk##name) vk->GetInstanceProcAddr(instance, "vk" #name);         \
		if (vk->name == NULL) {                                                          \
			error_log("Could not load instance level function: vk" #name);               \
			return false;                                                                \
		} else {                                                                         \
			debug_log("Successfully loaded instance level function: vk" #name);          \
		}

	#define INSTANCE_LEVEL_VULKAN_FUNCTION_FROM_EXTENSION(name, extension)               \
		for (size_t i = 0; i < extensions_count; i++) {                                  \
			if (strcmp(#extension, loaded_extensions[i]) == 0) {                         \
				vk->name = (PFN_vk##name) vk->GetInstanceProcAddr(instance, "vk" #name); \
				if (vk->name == NULL) {                                                  \
					error_log("Could not load instance level function: vk" #name);       \
					return false;                                                        \
				} else {                                                                 \
					debug_log("Successfully loaded instance level function: vk" #name);  \
				}                                                                        \
			}                                                                            \
		}

	#include "list.inl"

	return true;
}

bool load_device_level_functions(vk_functions *vk, VkDevice device,
	const char loaded_extensions[MAX_VULKAN_EXTENSIONS][VK_MAX_EXTENSION_NAME_SIZE],
	uint32_t extensions_count)
{
	#define DEVICE_LEVEL_VULKAN_FUNCTION(name)                                           \
		vk->name = (PFN_vk##name) vk->GetDeviceProcAddr(device, "vk" #name);             \
		if (vk->name == NULL) {                                                          \
			error_log("Could not load device level function: vk" #name);                 \
			return false;                                                                \
		} else {                                                                         \
			debug_log("Successfully loaded device level function: vk" #name);            \
		}

	#define DEVICE_LEVEL_VULKAN_FUNCTION_FROM_EXTENSION(name, extension)                 \
		for (size_t i = 0; i < extensions_count; i++) {                                  \
			if (strcmp(#extension, loaded_extensions[i]) == 0) {                         \
				vk->name = (PFN_vk##name) vk->GetDeviceProcAddr(device, "vk" #name);     \
				if (vk->name == NULL) {                                                  \
					error_log("Could not load device level function: vk" #name);         \
					return false;                                                        \
				} else {                                                                 \
					debug_log("Successfully loaded device level function: vk" #name);    \
				}                                                                        \
			}                                                                            \
		}

	#include "list.inl"
	
	return true;	
}

