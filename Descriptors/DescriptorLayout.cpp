#include "DescriptorLayout.h"


void DescriptorLayout::DestroyDescriptorSetLayout() {
    vkDestroyDescriptorSetLayout(deviceRef.GetDevice(), m_DescriptorSetLayout, nullptr);
}

void DescriptorLayout::CreateDescriptorSetLayout(const std::vector<VkDescriptorSetLayoutBinding>& bindings) {

    VkDescriptorSetLayoutCreateInfo createLayoutInfo = {};
    createLayoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
    createLayoutInfo.bindingCount = bindings.size();
    createLayoutInfo.pBindings = bindings.data();


    if(vkCreateDescriptorSetLayout(deviceRef.GetDevice(), &createLayoutInfo, nullptr, &m_DescriptorSetLayout) != VK_SUCCESS){
        throw std::runtime_error("failedd to create descriptor set layout");
    }

}