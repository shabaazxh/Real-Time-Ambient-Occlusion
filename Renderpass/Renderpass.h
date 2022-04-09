
#include <vulkan/vulkan.h>
#include <vector>
#include <array>
#include "../Device/Device.h"
#include <iostream>

class Renderpass {
    public:

    Renderpass(Device& device) : deviceRef{device} {}
    void CreateRenderpass(std::vector<VkAttachmentDescription> attachments, VkImageLayout depthLayout);
    void CreateRenderpass(std::vector<VkAttachmentDescription> attachments);
    void DestroyRenderpass();
    static VkAttachmentDescription createAttachment(VkFormat format, VkImageLayout initialLayout, VkImageLayout finalLayout, VkAttachmentLoadOp loadOp);


    VkRenderPass GetRenderpass() const { return m_Renderpass; }

    private:
    Device& deviceRef;
    VkRenderPass m_Renderpass;
};