#pragma once
#include <vulkan/vulkan.h>
#include <stdexcept>
#include <vector>


struct Memory {

    //(Overvoorde, 2016)

    // Finding the right type of memory to use based on our requirements
   static uint32_t FindMemoryType(const VkPhysicalDevice& pDevice, uint32_t typeFilter, VkMemoryPropertyFlags properties) {
       
       // find all available memory types that we have
       VkPhysicalDeviceMemoryProperties memoryProperties;
       vkGetPhysicalDeviceMemoryProperties(pDevice, &memoryProperties);

        // find a memory type that let's us store our vertex information but also lets us write to it from CPU
        for(uint32_t i = 0; i < memoryProperties.memoryTypeCount; i++) {
            // loop over memory types and check if the bit is set
            if((typeFilter & (1 << i)) && (memoryProperties.memoryTypes[i].propertyFlags & properties) == properties){
                return i;
            }
        }

        throw std::runtime_error("failed to find suitable memory type for required buffer operation");

    }
};