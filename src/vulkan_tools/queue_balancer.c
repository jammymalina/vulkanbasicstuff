#include "queue_balancer.h"

#include <string.h>
#include <stddef.h>
#include "../logger/logger.h"
#include "utils.h"
#include "../math/math_function.h"

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
		if (queue_props->queueCount > MAX_QUEUE_COUNT) {
			error_log("Not enough space for the queues");
			return false;
		}
		init_default_queue_config(&qb->queue_families[i]);
		qb->queue_families[i].properties = queue_props[i];
		qb->queue_families[i].queue_family_index = i; 
		qb->queue_families[i].queue_count = queue_props[i].queueCount;
		
		float priorities[MAX_QUEUE_COUNT];
		float_constant_range(priorities, 1.0, qb->queue_families[i].queue_count);
		memcpy(qb->queue_families[i].priorities, priorities, 
			qb->queue_families[i].queue_count * sizeof(float));
	}	

	qb->queue_families_count = queue_props_count;

	return true;
}

