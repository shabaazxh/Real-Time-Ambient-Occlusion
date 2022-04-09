#include "Framebuffer.h"


VkFramebuffer Framebuffer::CreateFramebuffer(VkFramebuffer framebuffer,VkDevice device, std::vector<VkImageView> imageView, VkRenderPass renderpass, VkExtent2D extent){


    //imageView.push_back(RenderData::depthResources.DepthImageView);

    VkFramebufferCreateInfo framebufferInfo = {};
    framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
    framebufferInfo.renderPass = renderpass;
    framebufferInfo.attachmentCount = static_cast<uint32_t>(imageView.size());
    framebufferInfo.pAttachments = imageView.data();
    framebufferInfo.width = extent.width;
    framebufferInfo.height = extent.height;
    framebufferInfo.layers = 1;

    if (vkCreateFramebuffer(device, &framebufferInfo, nullptr, &framebuffer) != VK_SUCCESS) {
        throw std::runtime_error("failed to create swap chain framebuffer");
    }

    return framebuffer;
}

void Framebuffer::DestroyFramebuffer()
{
    if(m_framebuffer != VK_NULL_HANDLE){
        vkDestroyFramebuffer(deviceRef.GetDevice(), m_framebuffer, nullptr);
    }
}

void Framebuffer::CreateFramebuffer(std::vector<VkImageView> imageView, VkRenderPass renderpass, VkExtent2D extent, bool useDepth) {
    
    if(useDepth)
    {
        imageView.push_back(RenderData::depthResources.DepthImageView);
    }
    
    VkFramebufferCreateInfo framebufferInfo = {};
    framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
    framebufferInfo.renderPass = renderpass;
    framebufferInfo.attachmentCount = static_cast<uint32_t>(imageView.size());
    framebufferInfo.pAttachments = imageView.data();
    framebufferInfo.width = extent.width;
    framebufferInfo.height = extent.height;
    framebufferInfo.layers = 1;

    if (vkCreateFramebuffer(deviceRef.GetDevice(), &framebufferInfo, nullptr, &m_framebuffer) != VK_SUCCESS) {
        throw std::runtime_error("failed to create swap chain framebuffer");
    }
}