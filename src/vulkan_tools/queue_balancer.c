#include "queue_balancer.h"

#include <string.h>
#include <stddef.h>
#include "../logger/logger.h"
#include "utils.h"
#include "../math/math_function.h"

static bool wait_for_queue(queue_container *queue, const vk_functions *vk) {
	return vk->QueueWaitIdle(queue->handle) == VK_SUCCESS;
}

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
		init_queue_container(&qb->queues[i], VK_NULL_HANDLE, 0);
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
			VkQueue q;
			get_device_queue(vk, device, qb->queue_families[i].queue_family_index, j, &q); 
			init_queue_container(&qb->queues[queue_count], q, i);
			VkQueueFlagBits flags = qb->queue_families[i].properties.queueFlags;	
			if (flags & VK_QUEUE_GRAPHICS_BIT) {
				qi_add(&qb->graphics_queue_indices, queue_count);
			}	
			if (flags & VK_QUEUE_COMPUTE_BIT) {
				qi_add(&qb->compute_queue_indices, queue_count);
			}
			if (flags & VK_QUEUE_TRANSFER_BIT) {
				qi_add(&qb->transfer_queue_indices, queue_count);
			}
			if (flags & VK_QUEUE_SPARSE_BINDING_BIT) {
				qi_add(&qb->sparse_queue_indices, queue_count);
			}
			queue_count++;
		}
	}	
	qb->queue_count = queue_count;
	return qb->queue_count > 0;
}

void init_queue_container(queue_container *queue, VkQueue vk_queue, uint32_t family_index) {
	queue->handle = vk_queue; 
	queue->family_index = family_index;
}

bool load_presentation_queues(const vk_functions *vk, queue_balancer *qb, VkPhysicalDevice physical_device, VkSurfaceKHR surface) {
	for (size_t i = 0; i < qb->queue_count; i++) {
		VkBool32 presentation_supported = VK_FALSE;
		VkResult result = vk->GetPhysicalDeviceSurfaceSupportKHR(physical_device, 
			qb->queues[i].family_index, surface, &presentation_supported);
		if (result == VK_SUCCESS && presentation_supported) {
			qi_add(&qb->presentation_queue_indices, i);
		}
	}
	return qb->presentation_queue_indices.size > 0;
}

bool submit_command_buffers_to_queue(queue_balancer *qb, uint32_t queue_index, 
	command_buffer *buf, const vk_functions *vk, 
	vk_semaphore wait_semaphores[MAX_SEMAPHORE_COUNT], uint32_t wait_semaphore_count, 
	vk_semaphore signal_semaphores[MAX_SEMAPHORE_COUNT], uint32_t signal_semaphore_count, 
	vk_fence *fence)
{
	if (queue_index >= qb->queue_count) {
		return false;
	}

    VkSemaphore wait_semaphore_handles[MAX_SEMAPHORE_COUNT];
    VkPipelineStageFlags wait_stages[MAX_SEMAPHORE_COUNT];
    for (size_t i = 0; i < wait_semaphore_count; i++) {
        wait_semaphore_handles[i] = wait_semaphores[i].handle;
        wait_stages[i] = wait_semaphores[i].waiting_stage;
	}

	VkSemaphore signal_semaphore_handles[MAX_SEMAPHORE_COUNT];
	for (size_t i = 0; i < signal_semaphore_count; i++) {
		signal_semaphore_handles[i] = signal_semaphores[i].handle;
	}

    VkSubmitInfo submit_info = {
        .sType                = VK_STRUCTURE_TYPE_SUBMIT_INFO,
        .pNext                = NULL,
        .waitSemaphoreCount   = wait_semaphore_count,
        .pWaitSemaphores      = wait_semaphore_handles,
        .pWaitDstStageMask    = wait_stages,
        .commandBufferCount   = buf->buffer_count,
        .pCommandBuffers      = buf->buffers,
        .signalSemaphoreCount = signal_semaphore_count,
        .pSignalSemaphores    = signal_semaphore_handles
	};
	
	return vk->QueueSubmit(qb->queues[queue_index].handle, 1, &submit_info, fence->handle) == VK_SUCCESS;
}

