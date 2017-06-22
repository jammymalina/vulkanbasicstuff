#ifndef VULKAN_FUNCTIONS
#define VULKAN_FUNCTIONS

#include <vulkan/vulkan.h>

typedef struct {
	#define EXPORTED_VULKAN_FUNCTION(name) PFN_vk##name name; 
	#define GLOBAL_LEVEL_VULKAN_FUNCTION(name) PFN_vk##name name; 
	#define INSTANCE_LEVEL_VULKAN_FUNCTION(name) PFN_vk##name name; 
	#define INSTANCE_LEVEL_VULKAN_FUNCTION_FROM_EXTENSION(name, extension) PFN_vk##name name; 
	#define DEVICE_LEVEL_VULKAN_FUNCTION(name) PFN_vk##name name; 
	#define DEVICE_LEVEL_VULKAN_FUNCTION_FROM_EXTENSION(name, extension) PFN_vk##name name;

	#include "list.inl" 
} vk_functions;	

#endif // VULKAN_FUNCTIONS

