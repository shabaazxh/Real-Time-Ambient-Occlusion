#pragma once
#include <vulkan/vulkan.h>
#include <stdexcept>
#include <vector>


struct Memory {

    // Finding the right type of memory to use based on our requirements
   static uint32_t FindMemoryType(const VkPhysicalDevice& pDevice, uint32_t typeFilter, VkMemoryPropertyFlags properties) {
       VkPhysicalDeviceMemoryProperties memoryProperties;
       vkGetPhysicalDeviceMemoryProperties(pDevice, &memoryProperties);

        for(uint32_t i = 0; i < memoryProperties.memoryTypeCount; i++) {
            if((typeFilter & (1 << i)) && (memoryProperties.memoryTypes[i].propertyFlags & properties) == properties){
                return i;
            }
        }

        throw std::runtime_error("failed to find suitable memory type for required buffer operation");

    }
};