#include "Buffer.h"


void Buffer::DestroyBuffer() {

    if(!m_UniformBuffers.empty() && !m_UniformBufferMemory.empty()){
        for(size_t i = 0; i < m_UniformBuffers.size(); i++){
            vkDestroyBuffer(deviceRef.GetDevice(), m_UniformBuffers[i], nullptr);
            vkFreeMemory(deviceRef.GetDevice(), m_UniformBufferMemory[i], nullptr);
        }
    }

    if(m_Buffer != VK_NULL_HANDLE){
        vkDestroyBuffer(deviceRef.GetDevice(), m_Buffer, nullptr);
    }

    if(m_BufferMemory != VK_NULL_HANDLE) {
        vkFreeMemory(deviceRef.GetDevice(), m_BufferMemory, nullptr);
    }
}

void Buffer::CopyBufferContents(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size){

    VkCommandBuffer commandBuffer = BeginSingleTimeCommands(deviceRef, m_CommandPool);
   
    VkBufferCopy region_to_copy{};
    region_to_copy.size = size;
    vkCmdCopyBuffer(commandBuffer, srcBuffer, dstBuffer, 1, &region_to_copy);

    EndSingleTimeCommands(deviceRef, commandBuffer, m_CommandPool);
}

void Buffer::CreateBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory){

    VkBufferCreateInfo bufferInfo = {};
    bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    bufferInfo.size = size; // size of the buffer
    bufferInfo.usage = usage; // usage specifies which purpose the buffer is going to be utilised for 
    bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE; //exclusive sine it's only going to be used by the graphics queue

    if(vkCreateBuffer(deviceRef.GetDevice(), &bufferInfo, nullptr, &buffer) != VK_SUCCESS) {
        throw std::runtime_error("Failed to create vertex buffer.");
    }

    // Once buffer is created, memory needs to be assigned for it 
    VkMemoryRequirements memRequirements;
    vkGetBufferMemoryRequirements(deviceRef.GetDevice(), buffer, &memRequirements);

    VkMemoryAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    allocInfo.allocationSize = memRequirements.size;
    allocInfo.memoryTypeIndex = Memory::FindMemoryType(deviceRef.GetPhysicalDevice(), memRequirements.memoryTypeBits, properties);

    if(vkAllocateMemory(deviceRef.GetDevice(), &allocInfo, nullptr, &bufferMemory) != VK_SUCCESS) {
        throw std::runtime_error("failed to allocate buffer memory");
    }    
    // Associate the allocated memory to the buffer
    vkBindBufferMemory(deviceRef.GetDevice(), buffer, bufferMemory, 0);
}

void Buffer::CreateVertexBuffer(std::vector<Vertex> primitiveData) {

    VkDeviceSize bufferSize = sizeof(primitiveData[0]) * primitiveData.size();

    // Use host visible buffer as temporary buffer 
    // CPU cannot directly write to a DEVICE_LOCAL buffer, we first write to a CPU visible buffer and then copy it's contents to a GPU one
    VkBuffer tempBuffer;
    VkDeviceMemory tempBufferMemory;
    CreateBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT |
    VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, tempBuffer, tempBufferMemory);

    // This will map the memory to host
    void* data;
    vkMapMemory(deviceRef.GetDevice(), tempBufferMemory, 0, bufferSize, 0, &data);
    // move data to the mapped memory 
    memcpy(data, primitiveData.data(), (size_t) bufferSize);
    vkUnmapMemory(deviceRef.GetDevice(), tempBufferMemory);

    // Device local is actual vertex buffer -> copy to GPU VRAM for better GPU access to the buffer
    CreateBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, 
    VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, m_Buffer, m_BufferMemory);

    // copy contents of the CPU buffer to the GPU one
    CopyBufferContents(tempBuffer, m_Buffer, bufferSize);
    
    // Destroy the temporary buffer 
    vkDestroyBuffer(deviceRef.GetDevice(), tempBuffer, nullptr);
    vkFreeMemory(deviceRef.GetDevice(), tempBufferMemory, nullptr);
}

void Buffer::CreateUniformBuffer(VkDeviceSize size, size_t resize) {
    
    m_UniformBuffers.resize(resize);
    m_UniformBufferMemory.resize(resize);

    for(size_t i = 0; i < resize; i++) {
        CreateBuffer(size, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT |
        VK_MEMORY_PROPERTY_HOST_CACHED_BIT, m_UniformBuffers[i], m_UniformBufferMemory[i]);
    }
}