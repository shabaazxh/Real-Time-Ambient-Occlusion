#pragma once

#include <vulkan/vulkan.h>
#define GLFW_INCLUDE_VULKAN

#include <vector>
#include <array>
#include "../Device/Device.h"
#include "../Structs/Shaders.h"


class DescriptorPool {
    public:
    DescriptorPool(Device& deviceRef) : deviceRef{deviceRef} {}
    void CreateDescriptorPool(std::vector<VkDescriptorPoolSize> poolSize, uint32_t maxSets);
    void DestroyDescriptorPool();
    //void CreateDescriptorPool();
    const VkDescriptorPool& GetDescriptorPool() const { return m_DescriporPool; }
    private:
    Device& deviceRef;
    VkDescriptorPool m_DescriporPool;
};

static VkDescriptorPoolSize DefinePoolSize(VkDescriptorType type, uint32_t count){

    VkDescriptorPoolSize poolSize{};
    poolSize.type = type;
    poolSize.descriptorCount = count;

    return poolSize;
}