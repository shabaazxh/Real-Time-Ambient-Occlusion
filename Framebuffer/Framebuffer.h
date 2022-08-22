#include <vulkan/vulkan.h>
#include <vector>
#include <array>
#include "../Device/Device.h"
#include "../Renderer/RenderData.h"

class Framebuffer {
    public:
    Framebuffer(Device& deviceRef) : deviceRef{deviceRef} {}
    void DestroyFramebuffer();
    static VkFramebuffer CreateFramebuffer(VkFramebuffer framebuffer,VkDevice device, std::vector<VkImageView> imageView, VkRenderPass renderpass, VkExtent2D extent);

    void CreateFramebuffer(std::vector<VkImageView> imageView, VkRenderPass renderpass, VkExtent2D extent, bool useDepth);
    VkFramebuffer GetFramebuffer() const { return m_framebuffer; }
    
    private:
    Device& deviceRef;
    VkFramebuffer m_framebuffer;
};