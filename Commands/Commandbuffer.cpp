#include "Commandbuffer.h"


void Commandbuffer::CreateCommandbuffer(){
    m_commandBuffer.resize(2); // Resize to max_frames in flight

    VkCommandBufferAllocateInfo info = {};
    info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    info.commandPool = m_CommandPool;
    info.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    info.commandBufferCount = (uint32_t )m_commandBuffer.size();


    if(vkAllocateCommandBuffers(deviceRef.GetDevice(), &info, m_commandBuffer.data()) != VK_SUCCESS) {
        throw std::runtime_error("failed to allocate command buffer");
    }
}

