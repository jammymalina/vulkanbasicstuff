#ifndef QUEUE_BALANCER_H
#define QUEUE_BALANCER_H

#include <stdbool.h>
#include <vulkan/vulkan.h>
#include "vulkan_limits.h"
#include "../vulkan_functions/functions.h"
#include "vk_config.h"

typedef struct {
	queue_family_config queue_families[MAX_QUEUE_FAMILIES];
	uint32_t queue_families_count;
} queue_balancer;

void init_empty_queue_balancer(queue_balancer *qb);
bool create_queue_balancer(const vk_functions *vk, queue_balancer *qb, VkPhysicalDevice physical_device);

#endif // QUEUE_BALANCER_H

