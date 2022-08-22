#pragma once

#include <vulkan/vulkan.h>
#define GLFW_INCLUDE_VULKAN

#include <vector>
#include "../Device/Device.h"
#include "../Structs/Shaders.h"


class DescriptorLayout {
    public:

    DescriptorLayout(Device& deviceRef, const std::vector<VkDescriptorSetLayoutBinding>& bindings) : deviceRef{deviceRef} {
        CreateDescriptorSetLayout(bindings);
    }

    void DestroyDescriptorSetLayout();
    void CreateDescriptorSetLayout(const std::vector<VkDescriptorSetLayoutBinding>& bindings);
    
    VkDescriptorSetLayoutBinding CreateDescriptorBinding();
    VkDescriptorSetLayout GetDescriptorSetLayout() const { return m_DescriptorSetLayout; }
    
    private:
    Device& deviceRef;
    VkDescriptorSetLayout m_DescriptorSetLayout;
};