#ifndef VULKAN_LOGS_H
#define VULKAN_LOGS_H 

#include <vulkan/vulkan.h>

static inline uint32_t get_vulkan_version_major(uint32_t version) {
	return version >> 22;
}

static inline uint32_t get_vulkan_version_minor(uint32_t version) {
	return ((version) >> 12) & 0x3ff;
}

static inline uint32_t get_vulkan_version_patch(uint32_t version) {
	return version & 0xfff;
}

char* vulkan_version_to_string(uint32_t version);
const char* physical_device_type_to_string(VkPhysicalDeviceType type);
void physical_device_features_log(VkPhysicalDeviceFeatures *device_features); 
void physical_device_props_log(VkPhysicalDeviceProperties *device_props);
char* queue_flags_to_string(VkQueueFlags flags);
void queue_props_log(const VkQueueFamilyProperties *props);

#endif // VULKAN_LOGS_H

