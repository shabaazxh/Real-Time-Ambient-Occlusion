#include "../Renderpass/Renderpass.h"


void Renderpass::DestroyRenderpass() {
    if (m_Renderpass != VK_NULL_HANDLE)
    {
        vkDestroyRenderPass(deviceRef.GetDevice(), m_Renderpass, nullptr);
        m_Renderpass = VK_NULL_HANDLE;
    }
}

VkAttachmentDescription Renderpass::createAttachment(VkFormat format, VkImageLayout initialLayout, VkImageLayout finalLayout, VkAttachmentLoadOp loadOp) {
    
    VkAttachmentDescription colorAttachment{};
    colorAttachment.format = format;
    colorAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
    colorAttachment.loadOp = loadOp;
    colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
    colorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    colorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    colorAttachment.initialLayout = initialLayout;
    colorAttachment.finalLayout = finalLayout;

    return colorAttachment;
}  

void Renderpass::CreateRenderpass(std::vector<VkAttachmentDescription> attachments, VkImageLayout depthLayout, bool useColor) {

    std::vector<VkAttachmentReference> colorAttachmentReferences;
    for(size_t i = 0; i < attachments.size(); i++){

        if(attachments[i].finalLayout == VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL || attachments[i].finalLayout == VK_IMAGE_LAYOUT_PRESENT_SRC_KHR) { 
            colorAttachmentReferences.push_back(VkAttachmentReference { (unsigned int)i, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL} );
        }
    }

    VkAttachmentReference depthAttachmentRef{};
    depthAttachmentRef.attachment = static_cast<uint32_t>(colorAttachmentReferences.size());
    depthAttachmentRef.layout = depthLayout;

    VkSubpassDescription subpass{};
    subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
    if(useColor)
    {
        subpass.colorAttachmentCount = static_cast<uint32_t>(colorAttachmentReferences.size());
        subpass.pColorAttachments = colorAttachmentReferences.data();
    }
    subpass.pDepthStencilAttachment = &depthAttachmentRef;

    VkSubpassDependency dependency{};
    dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
    dependency.dstSubpass = 0;
    dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;;
    dependency.srcAccessMask = 0;
    dependency.dstStageMask =  VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
    dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;

	std::array<VkSubpassDependency, 1> dependencies = { dependency };

    VkRenderPassCreateInfo renderPassInfo{};
    renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
    renderPassInfo.attachmentCount = static_cast<uint32_t>(attachments.size());
    renderPassInfo.pAttachments = attachments.data();
    renderPassInfo.subpassCount = 1;
    renderPassInfo.pSubpasses = &subpass;
    renderPassInfo.dependencyCount = 1;
    renderPassInfo.pDependencies = dependencies.data();

    if(vkCreateRenderPass(deviceRef.GetDevice(), &renderPassInfo, nullptr, &m_Renderpass) != VK_SUCCESS){
        throw std::runtime_error("failed to create render pass");
    }

}

void Renderpass::CreateShadowRenderpass(std::vector<VkAttachmentDescription> attachments, VkImageLayout depthLayout, bool useColor)
{
    std::vector<VkAttachmentReference> colorAttachmentReferences;
    if(useColor)
    {
        for(size_t i = 0; i < attachments.size(); i++){

            if(attachments[i].finalLayout == VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL || attachments[i].finalLayout == VK_IMAGE_LAYOUT_PRESENT_SRC_KHR) { 
                colorAttachmentReferences.push_back(VkAttachmentReference { (unsigned int)i, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL} );
            }
        }
    }

    VkAttachmentReference depthAttachmentRef{};
    depthAttachmentRef.attachment = static_cast<uint32_t>(colorAttachmentReferences.size());
    depthAttachmentRef.layout = depthLayout;

    VkSubpassDescription subpass{};
    subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
    if(useColor)
    {
        subpass.colorAttachmentCount = static_cast<uint32_t>(colorAttachmentReferences.size());
        subpass.pColorAttachments = colorAttachmentReferences.data();
    } else {
        subpass.colorAttachmentCount = 0;
    }
    subpass.pDepthStencilAttachment = &depthAttachmentRef;

	VkSubpassDependency dependency{};
	dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
	dependency.dstSubpass = 0;
	dependency.srcStageMask = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
	dependency.dstStageMask = VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
	dependency.srcAccessMask = VK_ACCESS_SHADER_READ_BIT;
	dependency.dstAccessMask = VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;
	dependency.dependencyFlags = VK_DEPENDENCY_BY_REGION_BIT;

	VkSubpassDependency dependency2{};
	dependency2.srcSubpass = 0;
	dependency2.dstSubpass = VK_SUBPASS_EXTERNAL;
	dependency2.srcStageMask = VK_PIPELINE_STAGE_LATE_FRAGMENT_TESTS_BIT;
	dependency2.dstStageMask = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
	dependency2.srcAccessMask = VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;
	dependency2.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;
	dependency2.dependencyFlags = VK_DEPENDENCY_BY_REGION_BIT;

	std::array<VkSubpassDependency, 2> dependencies = { dependency, dependency2 };

    VkRenderPassCreateInfo renderPassInfo{};
    renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
    renderPassInfo.attachmentCount = static_cast<uint32_t>(attachments.size());
    renderPassInfo.pAttachments = attachments.data();
    renderPassInfo.subpassCount = 1;
    renderPassInfo.pSubpasses = &subpass;
    renderPassInfo.dependencyCount = static_cast<uint32_t>(dependencies.size());
    renderPassInfo.pDependencies = dependencies.data();

    if(vkCreateRenderPass(deviceRef.GetDevice(), &renderPassInfo, nullptr, &m_Renderpass) != VK_SUCCESS){
        throw std::runtime_error("failed to create render pass");
    } 
}

void Renderpass::CreateRenderpass(std::vector<VkAttachmentDescription> attachments) {

    std::vector<VkAttachmentReference> colorAttachmentReferences;
    for(size_t i = 0; i < attachments.size(); i++){

        if(attachments[i].finalLayout == VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL || attachments[i].finalLayout == VK_IMAGE_LAYOUT_PRESENT_SRC_KHR) { 
            colorAttachmentReferences.push_back(VkAttachmentReference { (unsigned int)i, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL} );
        }
    }

    VkSubpassDescription subpass{};
    subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
    subpass.colorAttachmentCount = static_cast<uint32_t>(colorAttachmentReferences.size());
    subpass.pColorAttachments = colorAttachmentReferences.data();

    VkSubpassDependency dependency{};
    dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
    dependency.dstSubpass = 0;
    dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
    dependency.srcAccessMask = 0;
    dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
    dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;

    VkRenderPassCreateInfo renderPassInfo{};
    renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
    renderPassInfo.attachmentCount = static_cast<uint32_t>(attachments.size());
    renderPassInfo.pAttachments = attachments.data();
    renderPassInfo.subpassCount = 1;
    renderPassInfo.pSubpasses = &subpass;
    renderPassInfo.dependencyCount = 1;
    renderPassInfo.pDependencies = &dependency;

    if(vkCreateRenderPass(deviceRef.GetDevice(), &renderPassInfo, nullptr, &m_Renderpass) != VK_SUCCESS){
        throw std::runtime_error("failed to create render pass");
    }

}