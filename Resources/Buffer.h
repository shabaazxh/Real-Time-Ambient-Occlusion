#pragma once

#include <vulkan/vulkan.h>
#include "../Device/Device.h"
#include "../Structs/Shaders.h"
#include "../Device/Device.h"
#include "../Commands/Commandbuffer.h"
#include "Memory.h"

#include <stdexcept>
#include <vector>

class Buffer {
    
public:
    Buffer(Device& deviceRef) : deviceRef{deviceRef} {}
    Buffer(VkCommandPool commandPool, Device& deviceRef) : m_CommandPool{commandPool}, deviceRef{deviceRef} {}
    void DestroyBuffer();
    void CreateVertexBuffer(std::vector<Vertex> primitiveData);
    void CreateBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties);
    void CreateBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& memory);
    void CopyBufferContents(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size);
    void CreateUniformBuffer(VkDeviceSize size, size_t resize);
    void map(VkDeviceSize size);
    void unmap();
    void bind();
   
    VkBuffer GetBuffer() const { return m_Buffer; }
    VkDeviceMemory GetBufferMemory() const { return m_BufferMemory; }


    std::vector<VkBuffer> GetUniformBuffers() const { return m_UniformBuffers; }
    std::vector<VkDeviceMemory> GetUniformBufferMemory() const { return  m_UniformBufferMemory; }

    void* mapped = nullptr;
    std::vector<VkDeviceMemory> m_UniformBufferMemory;
private:
    
    VkBuffer m_Buffer = VK_NULL_HANDLE;
    VkDeviceMemory m_BufferMemory = VK_NULL_HANDLE;
    

    std::vector<VkBuffer> m_UniformBuffers;
    

    VkCommandPool m_CommandPool;
    Device& deviceRef;
};