#include "queue_balancer.h"

#include <string.h>
#include <stddef.h>
#include "../logger/logger.h"
#include "utils.h"
#include "../math/math_function.h"

GENERATE_BASIC_LIST_SOURCE(queue_indices, qi, uint32_t, MAX_QUEUE_COUNT)

void get_device_queue(const vk_functions *vk, VkDevice device, uint32_t queue_family_index, 
	uint32_t queue_index, VkQueue *queue) 
{
	vk->GetDeviceQueue(device, queue_family_index, queue_index, queue);
}

void init_empty_queue_balancer(queue_balancer *qb) {
	qb->queue_families_count = 0;
	for (size_t i = 0; i < MAX_QUEUE_FAMILIES; i++) 
		init_default_queue_config(&qb->queue_families[i]);
	qb->queue_count = 0;
	for (size_t i = 0; i < MAX_QUEUE_COUNT; i++)
		init_queue_container(&qb->queues[i], NULL, 0);
}

bool create_queue_balancer(const vk_functions *vk, queue_balancer *qb, VkPhysicalDevice physical_device) {
	qb->queue_families_count = 0;
	VkQueueFamilyProperties queue_props[MAX_QUEUE_PROPS];
	uint32_t queue_props_count;
	bool success = get_available_queue_props(vk, physical_device, queue_props, 
		&queue_props_count);

	if (!success) {
		error_log("Unable to query avaliable queue family properties");
		return false; 
	}
	if (queue_props_count > MAX_QUEUE_FAMILIES) {
		error_log("Not enough space for the queue family properties");
		return false; 
	}	
	
	for (size_t i = 0; i < queue_props_count; i++) {
		if (queue_props->queueCount > MAX_QUEUE_COUNT_PER_FAMILY) {
			error_log("Not enough space for the queues");
			return false;
		}
		init_default_queue_config(&qb->queue_families[i]);
		qb->queue_families[i].properties = queue_props[i];
		qb->queue_families[i].queue_family_index = i; 
		qb->queue_families[i].queue_count = queue_props[i].queueCount;
		
		float priorities[MAX_QUEUE_COUNT_PER_FAMILY];
		float_constant_range(priorities, 1.0, qb->queue_families[i].queue_count);
		memcpy(qb->queue_families[i].priorities, priorities, 
			qb->queue_families[i].queue_count * sizeof(float));
	}	

	qb->queue_families_count = queue_props_count;

	return true;
}

bool load_queues(const vk_functions *vk, queue_balancer *qb, VkDevice device) {
	uint32_t queue_count = 0;
	for (uint32_t i = 0; i < qb->queue_families_count; i++) {
		for (uint32_t j = 0; j < qb->queue_families[i].queue_count; j++) {
			get_device_queue(vk, device, qb->queue_families[i].queue_family_index, 
				j, &qb->queues[queue_count].vk_queue); 
			VkQueueFlagBits flags = qb->queue_families[i].properties.queueFlags;	
			if (flags & VK_QUEUE_GRAPHICS_BIT) {
			}	
			if (flags & VK_QUEUE_COMPUTE_BIT) {
			}
			if (flags & VK_QUEUE_TRANSFER_BIT) {
			}
			if (flags & VK_QUEUE_SPARSE_BINDING_BIT) {
			}
			queue_count++;
		}
	}	
	qb->queue_count = queue_count;
	return qb->queue_count > 0;
}

void init_queue_container(queue_container *queue, VkQueue vk_queue, uint32_t family_index) {
	queue->vk_queue = vk_queue; 
	queue->family_index = family_index;
}

