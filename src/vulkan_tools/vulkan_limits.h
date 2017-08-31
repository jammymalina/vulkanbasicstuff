#ifndef VULKAN_LIMITS_H
#define VULKAN_LIMITS_H

#define MAX_VULKAN_EXTENSIONS 256
#define MAX_VULKAN_DEVICES 16
#define MAX_QUEUE_PROPS 512
#define STORE_FIELD_SIZE 512
#define MAX_QUEUE_NAME_SIZE 256
#define MAX_QUEUE_FAMILIES 128
#define MAX_QUEUE_COUNT_PER_FAMILY 128
#define MAX_QUEUE_COUNT MAX_QUEUE_COUNT_PER_FAMILY * MAX_QUEUE_FAMILIES
#define MAX_PRESENT_MODE_COUNT 64
#define MAX_SURFACE_FORMAT_COUNT 256
#define MAX_SWAPCHAIN_IMAGE_COUNT 256
#define MAX_PRESENT_INFO_COUNT 128
#define MAX_RENDERING_SEMAPHORE_COUNT 32
#define MAX_COMMAND_BUFFER_COUNT 64
#define MAX_FENCE_COUNT 512
#define MAX_SEMAPHORE_COUNT 512

#endif // VULKAN_LIMITS_H
