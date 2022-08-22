#include "DescriptorPool.h"


void DescriptorPool::DestroyDescriptorPool() {
    if(m_DescriporPool != VK_NULL_HANDLE){
        vkDestroyDescriptorPool(deviceRef.GetDevice(), m_DescriporPool, nullptr);
    }
}

// Descriptor pool needs to be created allocate descriptors
void DescriptorPool::CreateDescriptorPool(std::vector<VkDescriptorPoolSize> poolSize, uint32_t maxSets) {

    VkDescriptorPoolCreateInfo poolInfo{};
    poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
    poolInfo.poolSizeCount = static_cast<uint32_t>(poolSize.size());
    poolInfo.pPoolSizes = poolSize.data();
    poolInfo.maxSets = maxSets; // 40

    if(vkCreateDescriptorPool(deviceRef.GetDevice(), &poolInfo, nullptr, &m_DescriporPool) != VK_SUCCESS){
        throw std::runtime_error("failedd to create descriptor pool!");
    }

}
