#include "Pipelines.h"

void Pipeline::DestroyPipelineResources()
{
    if(m_pipeline != VK_NULL_HANDLE){
        vkDestroyPipeline(deviceRef.GetDevice(), m_pipeline, nullptr);
        m_pipeline = VK_NULL_HANDLE;
        
    }
    if(m_pipelineLayout != VK_NULL_HANDLE){
        vkDestroyPipelineLayout(deviceRef.GetDevice(), m_pipelineLayout, nullptr);
        m_pipelineLayout = VK_NULL_HANDLE;
    }
}

VkShaderModule Pipeline::CreateShaderModule(const std::vector<char>& code) {

    VkShaderModuleCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
    createInfo.codeSize = code.size();
    createInfo.pCode = reinterpret_cast<const uint32_t*>(code.data());

    VkShaderModule shaderModule;
    if(vkCreateShaderModule(deviceRef.GetDevice(), &createInfo, nullptr, &shaderModule) != VK_SUCCESS){
        throw std::runtime_error("failed to create shader module.");
    }

    return shaderModule;
}

VkPipelineInputAssemblyStateCreateInfo Pipeline::inputAssemblyStateInfo(VkPrimitiveTopology topology) {
    
    VkPipelineInputAssemblyStateCreateInfo inputAssembly{};
    inputAssembly.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
    inputAssembly.topology = topology;
    inputAssembly.primitiveRestartEnable = VK_FALSE;

    return inputAssembly;
}

VkViewport Pipeline::viewport(VkExtent2D extent){

    VkViewport viewportinfo{};
    viewportinfo.x = 0.0f;
    viewportinfo.y = 0.0f;
    viewportinfo.width = (float) extent.width;
    viewportinfo.height = (float) extent.height;
    viewportinfo.minDepth = 0.0f;
    viewportinfo.maxDepth = 1.0f;

    return viewportinfo;
}

VkRect2D Pipeline::scissor(VkExtent2D extent){

    VkRect2D scissor{};
    scissor.offset = {0, 0};
    scissor.extent = extent;

    return scissor;

}

 VkPipelineRasterizationStateCreateInfo Pipeline::rasterizer(VkBool32 depthClamp, VkBool32 discardEnable, VkPolygonMode polyModde, VkCullModeFlags cullmode) {
     
     VkPipelineRasterizationStateCreateInfo rasterizer{};
     rasterizer.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
     rasterizer.depthClampEnable = depthClamp;
     rasterizer.rasterizerDiscardEnable = discardEnable;
     rasterizer.polygonMode = polyModde;
     rasterizer.lineWidth = 1.0f;
     rasterizer.cullMode = cullmode;
     rasterizer.frontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE;
     rasterizer.depthBiasEnable = VK_FALSE;
     rasterizer.pNext = nullptr;

     return rasterizer;
    
 }


 VkPipelineMultisampleStateCreateInfo Pipeline::multisampling(VkSampleCountFlagBits rasterSamples){
     
     VkPipelineMultisampleStateCreateInfo multisampling{};
     multisampling.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
     multisampling.sampleShadingEnable = VK_FALSE;
     multisampling.rasterizationSamples = rasterSamples;
     multisampling.pNext = nullptr;

     return multisampling;
 }

 VkPipelineColorBlendAttachmentState Pipeline::blendState(VkBool32 blendEnable, VkBlendFactor srcBlendFactor, VkBlendFactor dstBlendFactor){

     VkPipelineColorBlendAttachmentState colourBlendAttachment{};
     colourBlendAttachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
     colourBlendAttachment.blendEnable = blendEnable;
     colourBlendAttachment.srcColorBlendFactor = srcBlendFactor;
     colourBlendAttachment.dstColorBlendFactor = dstBlendFactor;
     colourBlendAttachment.colorBlendOp = VK_BLEND_OP_ADD;
     colourBlendAttachment.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
     colourBlendAttachment.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;
     colourBlendAttachment.alphaBlendOp = VK_BLEND_OP_ADD;

     return colourBlendAttachment;
 }

VkPipelineColorBlendStateCreateInfo Pipeline::colorBlending(std::vector<VkPipelineColorBlendAttachmentState>& blendState){

    VkPipelineColorBlendStateCreateInfo colorBlending{};
    colorBlending.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
    colorBlending.logicOpEnable = VK_FALSE;
    colorBlending.logicOp = VK_LOGIC_OP_COPY;
    colorBlending.attachmentCount = static_cast<uint32_t>(blendState.size());
    colorBlending.pAttachments = blendState.data();
    colorBlending.pNext = nullptr;

    return colorBlending;
}

void Pipeline::CreateGraphicsPipeline(
std::string vertexShaderPath,
std::string fragmentShaderPath,
VkPipelineInputAssemblyStateCreateInfo& inputAssembly, 
VkViewport& viewport, 
VkRect2D& scissor,
VkPipelineRasterizationStateCreateInfo& rasterizer,
VkPipelineMultisampleStateCreateInfo& multisampling,
VkPipelineColorBlendStateCreateInfo& colorBlending,
VkRenderPass renderpass,
VkDescriptorSetLayout descriptorSetLayout,
bool useDescriptorSetLayout) {
    
    auto vertShaderCode = FileLoader::LoadFile(vertexShaderPath);
    auto fragShaderCode = FileLoader::LoadFile(fragmentShaderPath);

    VkShaderModule vertShaderModule = CreateShaderModule(vertShaderCode);
    VkShaderModule fragShaderModule = CreateShaderModule(fragShaderCode);

    // Shaders need to be assigned to specific pipeline stage to be used
    VkPipelineShaderStageCreateInfo vertShaderStageInfo{};
    vertShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    vertShaderStageInfo.stage = VK_SHADER_STAGE_VERTEX_BIT;
    vertShaderStageInfo.module = vertShaderModule;
    vertShaderStageInfo.pName = "main";

    VkPipelineShaderStageCreateInfo fragShaderStageInfo{};
    fragShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    fragShaderStageInfo.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
    fragShaderStageInfo.module = fragShaderModule;
    fragShaderStageInfo.pName = "main";

    VkPipelineShaderStageCreateInfo shaderStages[] = {vertShaderStageInfo, fragShaderStageInfo};

    auto bindingDescription = Vertex::GetBindingDescription();
    auto attributeDescriptions = Vertex::GetAttributeDescriptions();

    VkPipelineVertexInputStateCreateInfo vertexInputInfo{};
    vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
    vertexInputInfo.vertexBindingDescriptionCount = 1;
    vertexInputInfo.vertexAttributeDescriptionCount = static_cast<uint32_t>(attributeDescriptions.size());
    vertexInputInfo.pVertexBindingDescriptions = &bindingDescription;
    vertexInputInfo.pVertexAttributeDescriptions = attributeDescriptions.data();

    VkPipelineViewportStateCreateInfo viewportState{};
    viewportState.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
    viewportState.viewportCount = 1;
    viewportState.pViewports = &viewport;
    viewportState.scissorCount = 1;
    viewportState.pScissors = &scissor;
    viewportState.pNext = nullptr;

    VkPipelineDepthStencilStateCreateInfo depthStencil = {};
    depthStencil.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
    depthStencil.depthTestEnable = VK_TRUE;
    depthStencil.depthWriteEnable = VK_TRUE;
    depthStencil.depthCompareOp = VK_COMPARE_OP_LESS_OR_EQUAL;
    depthStencil.back.compareOp = VK_COMPARE_OP_ALWAYS;

    std::vector<VkDynamicState> dynamicStates = {
        VK_DYNAMIC_STATE_VIEWPORT,
        VK_DYNAMIC_STATE_LINE_WIDTH
    };

    VkPipelineDynamicStateCreateInfo dynamicState{};
    dynamicState.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
    dynamicState.dynamicStateCount = static_cast<uint32_t>(dynamicStates.size());
    dynamicState.pDynamicStates = dynamicStates.data();
    dynamicState.pNext = nullptr;

    VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
    pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
    pipelineLayoutInfo.pNext = nullptr;

    if(useDescriptorSetLayout){
        pipelineLayoutInfo.setLayoutCount = 1;
        pipelineLayoutInfo.pSetLayouts = &descriptorSetLayout;
    }

    if(vkCreatePipelineLayout(deviceRef.GetDevice(), &pipelineLayoutInfo, nullptr, &m_pipelineLayout) != VK_SUCCESS){
        throw std::runtime_error("failed to create pipeline layout!");
    }

    VkGraphicsPipelineCreateInfo pipelineInfo = {};
    pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
    pipelineInfo.stageCount = 2;
    pipelineInfo.pStages = shaderStages;
    pipelineInfo.pVertexInputState = &vertexInputInfo;
    pipelineInfo.pInputAssemblyState = &inputAssembly;
    pipelineInfo.pViewportState = &viewportState;
    pipelineInfo.pRasterizationState = &rasterizer;
    pipelineInfo.pMultisampleState = &multisampling;
    pipelineInfo.pDepthStencilState = &depthStencil;
    pipelineInfo.pColorBlendState = &colorBlending;
    pipelineInfo.pDynamicState = nullptr;
    pipelineInfo.layout = m_pipelineLayout;
    pipelineInfo.renderPass = renderpass;
    pipelineInfo.subpass = 0;
    pipelineInfo.basePipelineHandle = VK_NULL_HANDLE;
    pipelineInfo.basePipelineIndex = -1;
    pipelineInfo.pNext = nullptr;

    if(vkCreateGraphicsPipelines(deviceRef.GetDevice(), VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &m_pipeline) != VK_SUCCESS){
        throw std::runtime_error("failed to create graphics pipeline!");
    }

    vkDestroyShaderModule(deviceRef.GetDevice(), fragShaderModule, nullptr);
    vkDestroyShaderModule(deviceRef.GetDevice(), vertShaderModule, nullptr);
}

void Pipeline::CreateDepthResources(Device& deviceReference, VkExtent2D extent) {

    Image ImageResource(deviceReference);

    RenderData::depthResources.DepthFormat = FindDepthFormat(deviceReference);

    ImageResource.CreateImage(extent.width, extent.height, RenderData::depthResources.DepthFormat, VK_IMAGE_TILING_OPTIMAL, 
    VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT | VK_IMAGE_USAGE_SAMPLED_BIT | VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);

    RenderData::depthResources.DepthImage = ImageResource.GetImage();
    RenderData::depthResources.DepthImageMemory = ImageResource.GetImageMemory();

    VkImageViewCreateInfo depthImageViewCreateInfo  = Image::CreateImageView(RenderData::depthResources.DepthImage, VK_IMAGE_VIEW_TYPE_2D, RenderData::depthResources.DepthFormat, VK_IMAGE_ASPECT_DEPTH_BIT);

    if(vkCreateImageView(deviceReference.GetDevice(), &depthImageViewCreateInfo, nullptr, &RenderData::depthResources.DepthImageView) != VK_SUCCESS) {
        throw std::runtime_error("failed to create depth image view in Pipline");
    }
    
}