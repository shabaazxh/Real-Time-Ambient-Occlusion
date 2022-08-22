#pragma once

#include <vulkan/vulkan.h>
#define GLFW_INCLUDE_VULKAN

#include <vector>
#include <stdexcept>
#include "../Device/Device.h"
#include "../Resources/FileLoad.h"
#include "../Structs/Shaders.h"
#include "../Resources/Image.h"
#include "../Renderer/RenderData.h"

class Pipeline {
    public:
    Pipeline(Device& deviceRef, VkExtent2D swapChainExtent) : deviceRef{deviceRef}, m_SwapChainExtent{swapChainExtent} {}
    
    void DestroyPipelineResources();

    void CreateGraphicsPipeline(
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
        bool useDescriptorSetLayout
    );

    VkShaderModule CreateShaderModule(const std::vector<char>& code);
    

    //Fixed stages
    static VkPipelineInputAssemblyStateCreateInfo inputAssemblyStateInfo(VkPrimitiveTopology topology);
    static VkViewport viewport(VkExtent2D extent);
    static VkRect2D scissor(VkExtent2D extent);
    static VkPipelineRasterizationStateCreateInfo rasterizer(VkBool32 depthClamp, VkBool32 discardEnable, VkPolygonMode polyModde, VkCullModeFlags cullmode);
    static VkPipelineMultisampleStateCreateInfo multisampling(VkSampleCountFlagBits rasterSamples);
    static VkPipelineColorBlendAttachmentState blendState(VkBool32 blendEnable, VkBlendFactor srcBlendFactor, VkBlendFactor dstBlendFactor);
    static VkPipelineColorBlendStateCreateInfo colorBlending(std::vector<VkPipelineColorBlendAttachmentState>& blendState);

    VkPipeline GetGraphicsPipeline() const { return m_pipeline; }
    VkPipelineLayout GetPipelineLayout() const { return m_pipelineLayout; }
    
    static void CreateDepthResources(Device& deviceRef, VkExtent2D extent);

    private:
    Device& deviceRef;
    VkExtent2D m_SwapChainExtent;

    VkPipelineLayout m_pipelineLayout;
    VkPipeline m_pipeline;
};