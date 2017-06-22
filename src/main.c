#include <stdio.h>
#include "vulkan_functions/functions.h"
#include "vulkan_functions/function_loader.h"

int main() {
	LIBRARY_TYPE vulkan_lib = NULL;
	vk_functions vk;
	load_all_vulkan_functions(vulkan_lib, &vk);	
	FREE_LIB(vulkan_lib);
}
