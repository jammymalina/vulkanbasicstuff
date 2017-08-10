#ifndef QUEUE_BALANCER_H
#define QUEUE_BALANCER_H

#include <stdbool.h>
#include <vulkan/vulkan.h>
#include "vulkan_limits.h"
#include "../vulkan_functions/functions.h"
#include "vk_config.h"
#include "../collections/basic_list.h"

GENERATE_BASIC_LIST_HEADER(queue_indices, qi, uint32_t, MAX_QUEUE_COUNT, 0)

typedef struct {
	VkQueue vk_queue;
	uint32_t family_index;
} queue_container;

typedef struct {
	queue_family_config queue_families[MAX_QUEUE_FAMILIES];
	uint32_t queue_families_count;

	queue_container queues[MAX_QUEUE_COUNT];
	uint32_t queue_count;

	queue_indices computational_queues_indices;
	queue_indices graphics_queues_indices;
} queue_balancer;

void init_empty_queue_balancer(queue_balancer *qb);
bool create_queue_balancer(const vk_functions *vk, queue_balancer *qb, VkPhysicalDevice physical_device);
bool load_queues(const vk_functions *vk, queue_balancer *qb, VkDevice device);

void init_queue_container(queue_container *queue, VkQueue vk_queue, uint32_t family_index);

#endif // QUEUE_BALANCER_H

