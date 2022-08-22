#pragma once

#include <vulkan/vulkan.h>
#include <vector>
#include "../Device/Device.h"

class Commandbuffer {
    public:

    Commandbuffer(Device& deviceRef, VkCommandPool commandPool) : deviceRef{deviceRef}, m_CommandPool{ commandPool } {
        CreateCommandbuffer();
    }

    void CreateCommandbuffer();
    static void RecordCommandbuffer(VkCommandBuffer commandBuffer, VkRenderPass renderpass, VkFramebuffer framebuffer, VkExtent2D extent, VkPipeline graphicsPipeline);


    std::vector<VkCommandBuffer> GetCommandbuffer() const { return m_commandBuffer; }
    private:
    

    Device& deviceRef;
    VkCommandPool m_CommandPool;
    std::vector<VkCommandBuffer> m_commandBuffer;

};

static VkCommandBuffer BeginSingleTimeCommands(Device& deviceRef, VkCommandPool commandPool) {

    VkCommandBufferAllocateInfo allocInfo = {};
    allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    allocInfo.commandPool = commandPool;
    allocInfo.commandBufferCount = 1;

    VkCommandBuffer commandBuffer;
    vkAllocateCommandBuffers(deviceRef.GetDevice(), &allocInfo, &commandBuffer);

    VkCommandBufferBeginInfo beginInfo = {};
    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
    
    vkBeginCommandBuffer(commandBuffer, &beginInfo);

    return commandBuffer;
}

static void EndSingleTimeCommands(Device& deviceRef,VkCommandBuffer commandBuffer, VkCommandPool commandPool)
{
    vkEndCommandBuffer(commandBuffer);

    VkSubmitInfo submitInfo = {};
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = &commandBuffer;

    vkQueueSubmit(deviceRef.GetGraphicsQueue(), 1, &submitInfo, VK_NULL_HANDLE);
    vkQueueWaitIdle(deviceRef.GetGraphicsQueue());

    vkFreeCommandBuffers(deviceRef.GetDevice(), commandPool, 1, &commandBuffer);
}