#include "CommandPool.h"


void CommandPool::DestroyCommandPool() {
    vkDestroyCommandPool(deviceRef.GetDevice(), m_CommandPool, nullptr);
}

void CommandPool::CreateCommandPool() {
    QueueFamilyIndices queueFamilyIndices = deviceRef.FindQueueFamilies(deviceRef.GetPhysicalDevice(), windowRef.GetSurface());

    VkCommandPoolCreateInfo commandPoolInfo = {};
    commandPoolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
    commandPoolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
    commandPoolInfo.queueFamilyIndex = queueFamilyIndices.graphicsFamily.value();

    if(vkCreateCommandPool(deviceRef.GetDevice(), &commandPoolInfo, nullptr, &m_CommandPool) != VK_SUCCESS){
        throw std::runtime_error("failed to create command pool.");
    }

}