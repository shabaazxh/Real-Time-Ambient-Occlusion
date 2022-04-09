#include "DescriptorPool.h"


void DescriptorPool::DestroyDescriptorPool() {
    if(m_DescriporPool != VK_NULL_HANDLE){
        vkDestroyDescriptorPool(deviceRef.GetDevice(), m_DescriporPool, nullptr);
    }
}

void DescriptorPool::CreateDescriptorPool(std::vector<VkDescriptorPoolSize> poolSize, uint32_t maxSets) {

    // std::array<VkDescriptorPoolSize,2> poolSize{};
    // poolSize[0].type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    // poolSize[0].descriptorCount = 20;
    // poolSize[1].type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
    // poolSize[1].descriptorCount = 20;

    VkDescriptorPoolCreateInfo poolInfo{};
    poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
    poolInfo.poolSizeCount = static_cast<uint32_t>(poolSize.size());
    poolInfo.pPoolSizes = poolSize.data();
    poolInfo.maxSets = maxSets; // 40

    if(vkCreateDescriptorPool(deviceRef.GetDevice(), &poolInfo, nullptr, &m_DescriporPool) != VK_SUCCESS){
        throw std::runtime_error("failedd to create descriptor pool!");
    }

}
