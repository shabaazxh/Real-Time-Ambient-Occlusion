#pragma once
#include "Application.h"

void Application::run() {
    init_window();
    init_vulkan();
    mainLoop();
    cleanup_resources();
}

// initialise the main window
void Application::init_window()
{
    Window.CreateWindow("Vulkan Renderer");
}

// Set up all render passes 
void Application::setup_renderpass()
{
    Pipeline::CreateDepthResources(*vkDevice, swapChain->GetSwapChainExtent());
    VkAttachmentDescription depthAttachment = Renderpass::createAttachment(RenderData::depthResources.DepthFormat, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_DEPTH_STENCIL_READ_ONLY_OPTIMAL, VK_ATTACHMENT_LOAD_OP_CLEAR);

    VkAttachmentDescription sceneRenderColorAttachment = Renderpass::createAttachment(swapChain->GetSwapChainFormat(), VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL, VK_ATTACHMENT_LOAD_OP_CLEAR);
    SponzaSceneRenderpass = std::make_unique<Renderpass>(*vkDevice);
    SponzaSceneRenderpass->CreateRenderpass({sceneRenderColorAttachment, depthAttachment}, VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL, true);

    VkAttachmentDescription positionImageColorAttachment = Renderpass::createAttachment(VK_FORMAT_R32G32B32A32_SFLOAT, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL, VK_ATTACHMENT_LOAD_OP_CLEAR);
    VkAttachmentDescription normalImageColorAttachment = Renderpass::createAttachment(VK_FORMAT_R32G32B32A32_SFLOAT, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL, VK_ATTACHMENT_LOAD_OP_CLEAR);
    GBufferRenderPass = std::make_unique<Renderpass>(*vkDevice);
    GBufferRenderPass->CreateRenderpass({positionImageColorAttachment, normalImageColorAttachment, depthAttachment}, VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL, true);

    VkAttachmentDescription ssaoColorAttachment = Renderpass::createAttachment(VK_FORMAT_R16G16B16A16_SFLOAT, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL, VK_ATTACHMENT_LOAD_OP_CLEAR);
    SSAORenderpass = std::make_unique<Renderpass>(*vkDevice);
    SSAORenderpass->CreateRenderpass({ssaoColorAttachment});

    VkAttachmentDescription colorAttachment = Renderpass::createAttachment(swapChain->GetSwapChainFormat(), VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_PRESENT_SRC_KHR, VK_ATTACHMENT_LOAD_OP_CLEAR);
    SwapChainRenderPass = std::make_unique<Renderpass>(*vkDevice);
    SwapChainRenderPass->CreateRenderpass({colorAttachment});

    VkAttachmentDescription ImGuiColorAttachment = Renderpass::createAttachment(swapChain->GetSwapChainFormat(), VK_IMAGE_LAYOUT_PRESENT_SRC_KHR, VK_IMAGE_LAYOUT_PRESENT_SRC_KHR, VK_ATTACHMENT_LOAD_OP_LOAD);
    ImGuiRenderpass = std::make_unique<Renderpass>(*vkDevice);
    ImGuiRenderpass->CreateRenderpass({ImGuiColorAttachment});
    RenderData::ImGuiData.renderpass = ImGuiRenderpass->GetRenderpass();

    VkAttachmentDescription ssaoBlurAttachment = Renderpass::createAttachment(swapChain->GetSwapChainFormat(), VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL, VK_ATTACHMENT_LOAD_OP_CLEAR);
    SSAOBlurRenderpass = std::make_unique<Renderpass>(*vkDevice);
    SSAOBlurRenderpass->CreateRenderpass({ssaoBlurAttachment});

    LightingPassRenderpass = std::make_unique<Renderpass>(*vkDevice);
    LightingPassRenderpass->CreateRenderpass({sceneRenderColorAttachment});

    VkAttachmentDescription shadowDepthAttachment = Renderpass::createAttachment(RenderData::depthResources.DepthFormat, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL, VK_ATTACHMENT_LOAD_OP_CLEAR);
    ShadowMapRenderpass = std::make_unique<Renderpass>(*vkDevice);
    ShadowMapRenderpass->CreateShadowRenderpass({shadowDepthAttachment}, VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL, false);

    RenderData::SponzaData.renderpass = SponzaSceneRenderpass->GetRenderpass();
    RenderData::ssaoRenderData.renderpass = SSAORenderpass->GetRenderpass();
    RenderData::GbufferRenderData.renderpass = GBufferRenderPass->GetRenderpass();
    RenderData::SSAOBlur.renderpass = SSAOBlurRenderpass->GetRenderpass();
    RenderData::Lighting.renderpass = LightingPassRenderpass->GetRenderpass();
}

void Application::setup_image_resources()
{
    positionImage = std::make_unique<Image>(*vkDevice);
    positionImage->CreateImageResource(swapChain->GetSwapChainExtent().width, swapChain->GetSwapChainExtent().height, VK_FORMAT_R32G32B32A32_SFLOAT, VK_IMAGE_TILING_OPTIMAL,
    VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_SAMPLED_BIT | VK_IMAGE_USAGE_TRANSFER_SRC_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, VK_IMAGE_ASPECT_COLOR_BIT);

    normalImage = std::make_unique<Image>(*vkDevice);
    normalImage->CreateImageResource(swapChain->GetSwapChainExtent().width, swapChain->GetSwapChainExtent().height, VK_FORMAT_R32G32B32A32_SFLOAT, VK_IMAGE_TILING_OPTIMAL,
    VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_SAMPLED_BIT | VK_IMAGE_USAGE_TRANSFER_SRC_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, VK_IMAGE_ASPECT_COLOR_BIT);

    SSAOImage = std::make_unique<Image>(*vkDevice);
    SSAOImage->CreateImageResource(swapChain->GetSwapChainExtent().width, swapChain->GetSwapChainExtent().height, VK_FORMAT_R16G16B16A16_SFLOAT, VK_IMAGE_TILING_OPTIMAL,
    VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_SAMPLED_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, VK_IMAGE_ASPECT_COLOR_BIT);

    noiseTextureImage = std::make_unique<Image>(*vkDevice);
    noiseTextureImage->CreateImage(4, 4, VK_FORMAT_R32G32B32A32_SFLOAT, VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT | 
    VK_IMAGE_USAGE_SAMPLED_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);     
    noiseTextureImage->CreateSampler();  

    depthSampleImage = std::make_unique<Image>(*vkDevice);
    depthSampleImage->CreateImageResource(swapChain->GetSwapChainExtent().width, swapChain->GetSwapChainExtent().height, RenderData::depthResources.DepthFormat, VK_IMAGE_TILING_OPTIMAL, 
    VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT | VK_IMAGE_USAGE_SAMPLED_BIT | VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, VK_IMAGE_ASPECT_DEPTH_BIT);

    SponzaSceneRenderedImage = std::make_unique<Image>(*vkDevice);
    SponzaSceneRenderedImage->CreateImageResource(swapChain->GetSwapChainExtent().width, swapChain->GetSwapChainExtent().height, swapChain->GetSwapChainFormat(), VK_IMAGE_TILING_OPTIMAL,
    VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_SAMPLED_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, VK_IMAGE_ASPECT_COLOR_BIT);

    SSAOBlurRenderImage = std::make_unique<Image>(*vkDevice);
    SSAOBlurRenderImage->CreateImageResource(swapChain->GetSwapChainExtent().width, swapChain->GetSwapChainExtent().height, swapChain->GetSwapChainFormat(), VK_IMAGE_TILING_OPTIMAL,
    VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_SAMPLED_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, VK_IMAGE_ASPECT_COLOR_BIT);

    LightingPassImage = std::make_unique<Image>(*vkDevice);
    LightingPassImage->CreateImageResource(swapChain->GetSwapChainExtent().width, swapChain->GetSwapChainExtent().height, swapChain->GetSwapChainFormat(),VK_IMAGE_TILING_OPTIMAL,
    VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_SAMPLED_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, VK_IMAGE_ASPECT_COLOR_BIT);

    GenerateSSAONoise(*noiseTextureImage, VK_FORMAT_R32G32B32A32_SFLOAT, ssaoNoise, commandPool->GetCommandPool(), *vkDevice);    

}

void Application::setup_framebuffers()
{
    //setup_Render_pass();
    SponzaSceneFramebuffer = std::make_unique<Framebuffer>(*vkDevice);
    SponzaSceneFramebuffer->CreateFramebuffer({SponzaSceneRenderedImage->GetImageView()}, SponzaSceneRenderpass->GetRenderpass(), swapChain->GetSwapChainExtent(), true);

    GbufferFramebuffer = std::make_unique<Framebuffer>(*vkDevice);
    GbufferFramebuffer->CreateFramebuffer({positionImage->GetImageView(), normalImage->GetImageView()}, GBufferRenderPass->GetRenderpass(), swapChain->GetSwapChainExtent(), true);

    SSAOFramebuffer = std::make_unique<Framebuffer>(*vkDevice);
    SSAOFramebuffer->CreateFramebuffer({SSAOImage->GetImageView()}, SSAORenderpass->GetRenderpass(), swapChain->GetSwapChainExtent(), false);

    SSAOBlurAOFramebuffer = std::make_unique<Framebuffer>(*vkDevice);
    SSAOBlurAOFramebuffer->CreateFramebuffer({SSAOBlurRenderImage->GetImageView()}, SSAOBlurRenderpass->GetRenderpass(), swapChain->GetSwapChainExtent(), false);

    LightingPassFramebuffer = std::make_unique<Framebuffer>(*vkDevice);
    LightingPassFramebuffer->CreateFramebuffer({LightingPassImage->GetImageView()}, LightingPassRenderpass->GetRenderpass(), swapChain->GetSwapChainExtent(), false);

    RenderData::SponzaData.framebuffer = SponzaSceneFramebuffer->GetFramebuffer();
    RenderData::GbufferRenderData.framebuffer = GbufferFramebuffer->GetFramebuffer();
    RenderData::ssaoRenderData.framebuffer = SSAOFramebuffer->GetFramebuffer();
    RenderData::Lighting.framebuffer = LightingPassFramebuffer->GetFramebuffer();
    RenderData::SSAOBlur.framebuffer = SSAOBlurAOFramebuffer->GetFramebuffer();
}

void Application::setup_pipelines()
{
    VkPipelineInputAssemblyStateCreateInfo inputAssembly = Pipeline::inputAssemblyStateInfo(VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST);
    VkViewport viewport = Pipeline::viewport(swapChain->GetSwapChainExtent());
    VkRect2D scissor = Pipeline::scissor(swapChain->GetSwapChainExtent());
    VkPipelineRasterizationStateCreateInfo rasterizer = Pipeline::rasterizer(VK_FALSE, VK_FALSE, VK_POLYGON_MODE_FILL, VK_CULL_MODE_BACK_BIT);
    VkPipelineRasterizationStateCreateInfo overlayRasterizer = Pipeline::rasterizer(VK_FALSE, VK_FALSE, VK_POLYGON_MODE_FILL, VK_CULL_MODE_NONE);
    VkPipelineMultisampleStateCreateInfo multisampling = Pipeline::multisampling(VK_SAMPLE_COUNT_1_BIT);
    std::vector<VkPipelineColorBlendAttachmentState> blendState = {Pipeline::blendState(VK_TRUE, VK_BLEND_FACTOR_SRC_ALPHA, VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA)};
    VkPipelineColorBlendStateCreateInfo colorBlending = Pipeline::colorBlending(blendState);

    std::vector<VkPipelineColorBlendAttachmentState> GBufferColorBlendAttachments = {Pipeline::blendState(VK_TRUE, VK_BLEND_FACTOR_ONE, VK_BLEND_FACTOR_ZERO), Pipeline::blendState(VK_TRUE, VK_BLEND_FACTOR_ONE, VK_BLEND_FACTOR_ZERO)};
    VkPipelineColorBlendStateCreateInfo GBufferColorBlending = Pipeline::colorBlending(GBufferColorBlendAttachments);

    GBufferPipeline = std::make_unique<Pipeline>(*vkDevice, swapChain->GetSwapChainExtent());
    GBufferPipeline->CreateGraphicsPipeline("Shaders/GBuffer/gbuffervert.spv", "Shaders/GBuffer/gbufferfrag.spv", inputAssembly, viewport, scissor, rasterizer, multisampling, GBufferColorBlending, GBufferRenderPass->GetRenderpass(), uboDescriptorLayout->GetDescriptorSetLayout(), true);

    SponzaGraphicsPipline = std::make_unique<Pipeline>(*vkDevice, swapChain->GetSwapChainExtent());
    SponzaGraphicsPipline->CreateGraphicsPipeline("Shaders/basevert.spv", "Shaders/basefrag.spv", inputAssembly, viewport, scissor, rasterizer, multisampling, colorBlending, SponzaSceneRenderpass->GetRenderpass(), uboDescriptorLayout->GetDescriptorSetLayout(), true);

    std::vector<VkPipelineColorBlendAttachmentState> ssaoBlendState  = {Pipeline::blendState(VK_FALSE, VK_BLEND_FACTOR_ONE, VK_BLEND_FACTOR_ZERO)};
    VkPipelineColorBlendStateCreateInfo ssaocolorBlending = Pipeline::colorBlending(ssaoBlendState);
    SSAOPipeline = std::make_unique<Pipeline>(*vkDevice, swapChain->GetSwapChainExtent());
    SSAOPipeline->CreateGraphicsPipeline("Shaders/Postprocessing/SSAOvert.spv", "Shaders/Postprocessing/SSAOfrag.spv", inputAssembly, viewport, scissor, overlayRasterizer, multisampling, ssaocolorBlending, SSAORenderpass->GetRenderpass(), SSAOuboDescriptorLayout->GetDescriptorSetLayout(), true);

    HBAOPipeline = std::make_unique<Pipeline>(*vkDevice, swapChain->GetSwapChainExtent());
    HBAOPipeline->CreateGraphicsPipeline("Shaders/Postprocessing/SSAOvert.spv", "Shaders/Postprocessing/HBAO.spv", inputAssembly, viewport, scissor, overlayRasterizer, multisampling, ssaocolorBlending, SSAORenderpass->GetRenderpass(), SSAOuboDescriptorLayout->GetDescriptorSetLayout(), true);

    AlchemyPipeline = std::make_unique<Pipeline>(*vkDevice, swapChain->GetSwapChainExtent());
    AlchemyPipeline->CreateGraphicsPipeline("Shaders/Postprocessing/SSAOvert.spv", "Shaders/Postprocessing/AlchemyAO.spv", inputAssembly, viewport, scissor, overlayRasterizer, multisampling, ssaocolorBlending, SSAORenderpass->GetRenderpass(), SSAOuboDescriptorLayout->GetDescriptorSetLayout(), true);

    SSAOBlurPipeline = std::make_unique<Pipeline>(*vkDevice, swapChain->GetSwapChainExtent());
    SSAOBlurPipeline->CreateGraphicsPipeline("Shaders/Postprocessing/Quadvert.spv", "Shaders/Postprocessing/Blur.spv", inputAssembly, viewport, scissor, overlayRasterizer, multisampling, ssaocolorBlending, SSAOBlurRenderpass->GetRenderpass(), SSAOBlurDescriptorLayout->GetDescriptorSetLayout(), true);

    LightPassPipeline = std::make_unique<Pipeline>(*vkDevice, swapChain->GetSwapChainExtent());
    LightPassPipeline->CreateGraphicsPipeline("Shaders/Postprocessing/Lightingvert.spv", "Shaders/Postprocessing/Lightingfrag.spv", inputAssembly, viewport, scissor, overlayRasterizer, multisampling, colorBlending, LightingPassRenderpass->GetRenderpass(), LightingPassDescriptorLayout->GetDescriptorSetLayout(), true);

    VkExtent2D extent = {2048, 2048};
    VkViewport shadowViewport = Pipeline::viewport({2048, 2048});
    VkRect2D shadowScissor = Pipeline::scissor({2048, 2048});
    ShadowPipeline = std::make_unique<Pipeline>(*vkDevice, extent);
    ShadowPipeline->CreateGraphicsPipeline("Shaders/Postprocessing/Shadow.vert.spv", "Shaders/Postprocessing/Shadow.frag.spv", inputAssembly, shadowViewport, shadowScissor, overlayRasterizer, multisampling, colorBlending, ShadowMapRenderpass->GetRenderpass(), LightingPassDescriptorLayout->GetDescriptorSetLayout(), true);

    // Display Quad is final swapchain present 
    DisplayQuad = std::make_unique<Pipeline>(*vkDevice, swapChain->GetSwapChainExtent());
    DisplayQuad->CreateGraphicsPipeline("Shaders/Postprocessing/Quadvert.spv", "Shaders/Postprocessing/Quadfrag.spv", inputAssembly, viewport, scissor, overlayRasterizer, multisampling, colorBlending, SwapChainRenderPass->GetRenderpass(), DisplayDescriptorLayout->GetDescriptorSetLayout(), true);

    RenderData::SponzaData.pipeline = SponzaGraphicsPipline->GetGraphicsPipeline();
    RenderData::SponzaData.pipelineLayout = SponzaGraphicsPipline->GetPipelineLayout();

    RenderData::GbufferRenderData.pipeline = GBufferPipeline->GetGraphicsPipeline();
    RenderData::GbufferRenderData.pipelineLayout = GBufferPipeline->GetPipelineLayout();

    RenderData::ssaoRenderData.ssaoPipeline = SSAOPipeline->GetGraphicsPipeline();
    RenderData::ssaoRenderData.pipelineLayout = SSAOPipeline->GetPipelineLayout();
    RenderData::ssaoRenderData.hbaoPipeline = HBAOPipeline->GetGraphicsPipeline();
    RenderData::ssaoRenderData.alchchemyPipeline = AlchemyPipeline->GetGraphicsPipeline();

    RenderData::SSAOBlur.pipeline = SSAOBlurPipeline->GetGraphicsPipeline();
    RenderData::SSAOBlur.pipelineLayout = SSAOBlurPipeline->GetPipelineLayout();

    RenderData::Lighting.pipeline = LightPassPipeline->GetGraphicsPipeline();
    RenderData::Lighting.pipelineLayout = LightPassPipeline->GetPipelineLayout();

    RenderData::DisplayQuad.pipeline = DisplayQuad->GetGraphicsPipeline();
    RenderData::DisplayQuad.pipelineLayout = DisplayQuad->GetPipelineLayout();

    RenderData::Shadow.pipeline = ShadowPipeline->GetGraphicsPipeline();
    RenderData::Shadow.pipelineLayout = ShadowPipeline->GetPipelineLayout();
}

void Application::init_vulkan()
{
    create_instance();  
    RenderData::GlobalVK.instance = instance;
    Window.CreateSurface(instance);
    Window.LockWindowCamera(false);
    vkDevice->SetSurface(Window.GetSurface());
    vkDevice->SelectGPUDevice(instance);
    vkDevice->CreateDevice();

    commandPool = std::make_unique<CommandPool>(*vkDevice, Window);
    commandPool->CreateCommandPool();

    swapChain = std::make_unique<SwapChain>(*vkDevice, Window.GetSurface());
    swapChain->SetWindow(Window.GetWindow());

    // Create render passes
    setup_renderpass();
    
    // Image resources
    setup_image_resources();
    ShadowMapImage = std::make_unique<Image>(*vkDevice);
    ShadowMapImage->CreateImageResource(2048, 2048, RenderData::depthResources.DepthFormat, VK_IMAGE_TILING_OPTIMAL, 
    VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT | VK_IMAGE_USAGE_SAMPLED_BIT | VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, VK_IMAGE_ASPECT_DEPTH_BIT);

    // Framebuffers
    setup_framebuffers();

    swapChain->CreateSwapChainFramebuffers(SwapChainRenderPass->GetRenderpass());

    ShadowMapFramebuffer = std::make_unique<Framebuffer>(*vkDevice);
    ShadowMapFramebuffer->CreateFramebuffer({ShadowMapImage->GetImageView()}, ShadowMapRenderpass->GetRenderpass(), {2048, 2048}, false);

    descriptorPool = std::make_unique<DescriptorPool>(*vkDevice);
    VkDescriptorPoolSize uniformBuffers = DefinePoolSize(VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 20);
    VkDescriptorPoolSize uniformSamplers = DefinePoolSize(VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 20);
    descriptorPool->CreateDescriptorPool({uniformBuffers, uniformSamplers}, 40);

    ImGuiDescriptorPool = std::make_unique<DescriptorPool>(*vkDevice);
    ImGuiDescriptorPool->CreateDescriptorPool(ImGuiPoolSizes, 1000); 

    RenderData::GlobalVK.commandPool = commandPool->GetCommandPool();

    ImGuiUI = std::make_unique<UI>(instance, *vkDevice, Window);
    ImGuiUI->InitImGui(ImGuiDescriptorPool->GetDescriptorPool(), ImGuiRenderpass->GetRenderpass(), commandPool->GetCommandPool());

    // Buffers
    {
        Model SponzaModel("Assets/Sponza.obj");
        ModelBuffer = std::make_unique<Buffer>(commandPool->GetCommandPool(), *vkDevice);
        ModelBuffer->CreateVertexBuffer(SponzaModel.GetVertexData());
        ModelBuffer->CreateUniformBuffer(sizeof(CameraProjection), swapChain->GetSwapChainImages().size());

        SSAOBuffer = std::make_unique<Buffer>(commandPool->GetCommandPool(), *vkDevice);
        SSAOBuffer->CreateUniformBuffer(sizeof(SSAOubo), swapChain->GetSwapChainImages().size());
        
        SSAOQuadBuffer = std::make_unique<Buffer>(commandPool->GetCommandPool(), *vkDevice);
        SSAOQuadBuffer->CreateUniformBuffer(sizeof(CameraProjection), swapChain->GetSwapChainImages().size());
        SSAOQuadBuffer->CreateVertexBuffer(primitives.Quad);

        DisplayQuadBuffer = std::make_unique<Buffer>(commandPool->GetCommandPool(), *vkDevice);
        DisplayQuadBuffer->CreateVertexBuffer(primitives.Quad);
        DisplayQuadBuffer->CreateUniformBuffer(sizeof(RenderPresentSettings), swapChain->GetSwapChainImages().size());

        LightBuffer = std::make_unique<Buffer>(commandPool->GetCommandPool(), *vkDevice);
        LightBuffer->CreateUniformBuffer(sizeof(Light), swapChain->GetSwapChainImages().size());

        RenderData::SponzaData.vertexBuffer = ModelBuffer->GetBuffer();
        RenderData::SponzaData.vertexData = SponzaModel.GetVertexData();
        RenderData::SponzaData.UniformBuffers = ModelBuffer->GetUniformBuffers();
        RenderData::SponzaData.UniformMemory = ModelBuffer->GetUniformBufferMemory();

        RenderData::ssaoRenderData.UniformBuffers = SSAOBuffer->GetUniformBuffers();
        RenderData::ssaoRenderData.UniformMemory = SSAOBuffer->GetUniformBufferMemory();
        RenderData::ssaoRenderData.cameraUniformBuffers = SSAOQuadBuffer->GetUniformBuffers();
        RenderData::ssaoRenderData.cameraUniformMemory = SSAOQuadBuffer->GetUniformBufferMemory();
        RenderData::ssaoRenderData.vertexData = primitives.Quad;
        RenderData::ssaoRenderData.vertexBuffer = SSAOQuadBuffer->GetBuffer();

        RenderData::Lighting.UniformBuffers = LightBuffer->GetUniformBuffers();
        RenderData::Lighting.UniformMemory = LightBuffer->GetUniformBufferMemory();

        RenderData::Shadow.framebuffer = ShadowMapFramebuffer->GetFramebuffer();
        RenderData::Shadow.renderpass = ShadowMapRenderpass->GetRenderpass();

        RenderData::DisplayQuad.vertexBuffer = DisplayQuadBuffer->GetBuffer();
        RenderData::DisplayQuad.vertexData = primitives.Quad;
        RenderData::DisplayQuad.UniformBuffers = DisplayQuadBuffer->GetUniformBuffers();
        RenderData::DisplayQuad.UniformMemory = DisplayQuadBuffer->GetUniformBufferMemory();
    }

    setup_descriptor_sets();

    // Create graphics pipeline
    setup_pipelines();

    
    Commandbuffer renderingCommands(*vkDevice, commandPool->GetCommandPool());
    RenderData::GlobalVK.RenderingCommands = renderingCommands.GetCommandbuffer();
    Commandbuffer ImGuiCommands(*vkDevice, commandPool->GetCommandPool());
    RenderData::ImGuiData.ImGuiCmds = ImGuiCommands.GetCommandbuffer();

    Render = std::make_unique<Renderer>(*vkDevice, *swapChain, *ImGuiUI, Window, renderingCommands.GetCommandbuffer());
    Render->CreateSync();
    
}
// Generate the noise texture containing random vectors for SSAO
void Application::GenerateSSAONoise(Image& image, VkFormat format, std::vector<glm::vec4>& ssaoNoise, VkCommandPool commandPool, Device& deviceRef) {

    VkDeviceSize noiseImageSize = 4*4*4*sizeof(float);
    
    // Noise generation implemented following (Joey De Vries, 2020) implementation.
    std::uniform_real_distribution<float> randomFloats(0.0, 1.0);
    std::default_random_engine generator;

    for(unsigned int i = 0; i < 16; i++){
        glm::vec4 noise(randomFloats(generator) * 2.0 - 1.0, randomFloats(generator) * 2.0 - 1.0, 0.0, 1.0);
        ssaoNoise.push_back(noise);
    }

    VkBuffer tempStagingBuffer;
    VkDeviceMemory tempStagingBufferMemory;

    Buffer tempBuffer(commandPool, deviceRef);
    tempBuffer.CreateBuffer(noiseImageSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, 
    VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
    tempStagingBuffer, tempStagingBufferMemory);

    void* data;
    vkMapMemory(deviceRef.GetDevice(), tempStagingBufferMemory, 0, noiseImageSize, 0, &data);
    memcpy(data, ssaoNoise.data(), static_cast<size_t>(noiseImageSize));
    vkUnmapMemory(deviceRef.GetDevice(), tempStagingBufferMemory);

    image.TransitionImageLayout(commandPool, image.GetImage(), format, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);

    image.CopyBufferToImage(commandPool, tempStagingBuffer, image.GetImage(), 4, 4);

    image.TransitionImageLayout(commandPool, image.GetImage(), format, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);

    vkDestroyBuffer(deviceRef.GetDevice(), tempStagingBuffer, nullptr);
    vkFreeMemory(deviceRef.GetDevice(), tempStagingBufferMemory, nullptr);

    VkImageViewCreateInfo imageView =  Image::CreateImageView(image.GetImage(), VK_IMAGE_VIEW_TYPE_2D, VK_FORMAT_R32G32B32A32_SFLOAT, VK_IMAGE_ASPECT_COLOR_BIT);

    VkImageView view = image.GetImageView();

    if(vkCreateImageView(deviceRef.GetDevice(), &imageView, nullptr, &view) != VK_SUCCESS){
        throw std::runtime_error("failed to create noise image view.");
    }

    image.SetImageView(view);

    tempBuffer.DestroyBuffer();
}
// Call events that need to be updated every frame
void Application::mainLoop() 
{
    while(!glfwWindowShouldClose(Window.GetWindow()))
    {
        glfwSetCursorPosCallback(Window.GetWindow(), &MouseCallback);


        if(Render->GetSwapchainState() == true)
        {
            recreate_swapchain();
            vkDeviceWaitIdle(vkDevice->GetDevice());
            setup_image_resources();
            setup_framebuffers();
            setup_descriptor_sets();
            setup_pipelines();
            Render->DestroyRendererResources();
            Render->CreateSync();
            Render->SetRecreateSwapChain(false);
        }

        Render->UpdateCamera();
        Render->Renderframe();

        glfwPollEvents();
    }

    vkDeviceWaitIdle(vkDevice->GetDevice());
}

void Application::cleanup_swapchain()
{
    swapChain->DestroySwapChain();
}

void Application::cleanup_render_resources() {
    // Destroy Image resources
    positionImage->DestroyImageResources();
    normalImage->DestroyImageResources();
    SSAOImage->DestroyImageResources();
    noiseTextureImage->DestroyImageResources();
    depthSampleImage->DestroyImageResources();
    SponzaSceneRenderedImage->DestroyImageResources();
    SSAOBlurRenderImage->DestroyImageResources();
    LightingPassImage->DestroyImageResources();

    // DescriptorSetLayouts
    uboDescriptorLayout->DestroyDescriptorSetLayout();
    SSAOuboDescriptorLayout->DestroyDescriptorSetLayout();
    SSAOBlurDescriptorLayout->DestroyDescriptorSetLayout();
    DisplayDescriptorLayout->DestroyDescriptorSetLayout();
    LightingPassDescriptorLayout->DestroyDescriptorSetLayout();

    // pipelines
    GBufferPipeline->DestroyPipelineResources();
    SponzaGraphicsPipline->DestroyPipelineResources();
    SSAOPipeline->DestroyPipelineResources();
    HBAOPipeline->DestroyPipelineResources();
    AlchemyPipeline->DestroyPipelineResources();
    SSAOBlurPipeline->DestroyPipelineResources();
    LightPassPipeline->DestroyPipelineResources();
    DisplayQuad->DestroyPipelineResources();

    SponzaSceneFramebuffer->DestroyFramebuffer();
    GbufferFramebuffer->DestroyFramebuffer();
    SSAOFramebuffer->DestroyFramebuffer();
    SSAOBlurAOFramebuffer->DestroyFramebuffer();
    LightingPassFramebuffer->DestroyFramebuffer();

    // Render pass
    SwapChainRenderPass->DestroyRenderpass();
    SponzaSceneRenderpass->DestroyRenderpass();
    GBufferRenderPass->DestroyRenderpass();
    SSAORenderpass->DestroyRenderpass();
    ImGuiRenderpass->DestroyRenderpass();
    SSAOBlurRenderpass->DestroyRenderpass();
    LightingPassRenderpass->DestroyRenderpass();
}

// Swapchain re-creation for window re-sizing
void Application::recreate_swapchain()
{
    vkDeviceWaitIdle(vkDevice->GetDevice());
    cleanup_swapchain();
    swapChain->RecreateSwapChain(swapChain->GetSwapChain());
    cleanup_render_resources();
    setup_renderpass();
    swapChain->CreateSwapChainFramebuffers(SwapChainRenderPass->GetRenderpass()); 
}

void Application::cleanup_resources()
{

    cleanup_swapchain();
    
    descriptorPool->DestroyDescriptorPool();

    RenderData::depthResources.Destroy(*vkDevice);

    // Destroy Image resources
    positionImage->DestroyImageResources();
    normalImage->DestroyImageResources();
    SSAOImage->DestroyImageResources();
    noiseTextureImage->DestroyImageResources();
    depthSampleImage->DestroyImageResources();
    SponzaSceneRenderedImage->DestroyImageResources();
    SSAOBlurRenderImage->DestroyImageResources();
    LightingPassImage->DestroyImageResources();
    ShadowMapImage->DestroyImageResources();

    // DescriptorSetLayouts
    uboDescriptorLayout->DestroyDescriptorSetLayout();
    SSAOuboDescriptorLayout->DestroyDescriptorSetLayout();
    SSAOBlurDescriptorLayout->DestroyDescriptorSetLayout();
    DisplayDescriptorLayout->DestroyDescriptorSetLayout();
    LightingPassDescriptorLayout->DestroyDescriptorSetLayout();

    // Destroy buffers
    ModelBuffer->DestroyBuffer();
    SSAOBuffer->DestroyBuffer();
    SSAOQuadBuffer->DestroyBuffer();
    DisplayQuadBuffer->DestroyBuffer();
    LightBuffer->DestroyBuffer();

    // Destroy framebuffers
    SponzaSceneFramebuffer->DestroyFramebuffer();
    GbufferFramebuffer->DestroyFramebuffer();
    SSAOFramebuffer->DestroyFramebuffer();
    SSAOBlurAOFramebuffer->DestroyFramebuffer();
    LightingPassFramebuffer->DestroyFramebuffer();
    ShadowMapFramebuffer->DestroyFramebuffer();

    // pipelines
    GBufferPipeline->DestroyPipelineResources();
    SponzaGraphicsPipline->DestroyPipelineResources();
    SSAOPipeline->DestroyPipelineResources();
    HBAOPipeline->DestroyPipelineResources();
    AlchemyPipeline->DestroyPipelineResources();
    SSAOBlurPipeline->DestroyPipelineResources();
    LightPassPipeline->DestroyPipelineResources();
    ShadowPipeline->DestroyPipelineResources();
    DisplayQuad->DestroyPipelineResources();

    // Render pass
    SwapChainRenderPass->DestroyRenderpass();
    SponzaSceneRenderpass->DestroyRenderpass();
    GBufferRenderPass->DestroyRenderpass();
    SSAORenderpass->DestroyRenderpass();
    ImGuiRenderpass->DestroyRenderpass();
    SSAOBlurRenderpass->DestroyRenderpass();
    LightingPassRenderpass->DestroyRenderpass();
    ShadowMapRenderpass->DestroyRenderpass();

    Render->DestroyRendererResources();
    commandPool->DestroyCommandPool();
    ImGuiDescriptorPool->DestroyDescriptorPool();

    ImGui_ImplVulkan_Shutdown();
            
    vkDevice->DestroyDevice();
    Window.DestroySurface(instance);
    vkDestroyInstance(instance, nullptr);
    Window.DestroyWindow();
    glfwTerminate();
}

// Set up the descriptor sets data
void Application::setup_descriptor_sets() {
    // Sponza
    std::vector<VkDescriptorSetLayoutBinding> SponzaBindings =
    {
        CreateDescriptorBinding(0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_VERTEX_BIT)
    };
    uboDescriptorLayout = std::make_unique<DescriptorLayout>(*vkDevice, SponzaBindings); //Create descriptor layout
    DesciptorSetAllocate uboDescriptorSetAllocate(*vkDevice, descriptorPool->GetDescriptorPool(), uboDescriptorLayout->GetDescriptorSetLayout(), RenderData::SponzaData.descriptorSets, 3); //Allocate descriptors using layout
    UpdateDescriptorSet(0, *vkDevice, RenderData::SponzaData.UniformBuffers, RenderData::SponzaData.descriptorSets, sizeof(CameraProjection), VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER);

    // SSAO
    std::vector<VkDescriptorSetLayoutBinding> SSAOBindings =
    {
        CreateDescriptorBinding(0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_FRAGMENT_BIT),
        CreateDescriptorBinding(1, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, VK_SHADER_STAGE_FRAGMENT_BIT),
        CreateDescriptorBinding(2, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, VK_SHADER_STAGE_FRAGMENT_BIT),
        CreateDescriptorBinding(3, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, VK_SHADER_STAGE_FRAGMENT_BIT),
        CreateDescriptorBinding(4, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT)
        };
    SSAOuboDescriptorLayout = std::make_unique<DescriptorLayout>(*vkDevice, SSAOBindings);
    DesciptorSetAllocate SSAOuboDescriptorSetAllocate(*vkDevice, descriptorPool->GetDescriptorPool(), SSAOuboDescriptorLayout->GetDescriptorSetLayout(), RenderData::ssaoRenderData.descriptorSets, 3); //Allocate descriptors using layout
    UpdateDescriptorSet(0, *vkDevice, RenderData::ssaoRenderData.UniformBuffers, RenderData::ssaoRenderData.descriptorSets, sizeof(SSAOubo), VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER);
    UpdateDescriptorSet(1, *vkDevice, RenderData::ssaoRenderData.descriptorSets, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, noiseTextureImage->GetImageView(), noiseTextureImage->GetSampler(), VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);
    UpdateDescriptorSet(2, *vkDevice, RenderData::ssaoRenderData.descriptorSets, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, positionImage->GetImageView(), noiseTextureImage->GetSampler(), VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);
    UpdateDescriptorSet(3, *vkDevice, RenderData::ssaoRenderData.descriptorSets, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, RenderData::depthResources.DepthImageView, noiseTextureImage->GetSampler(), VK_IMAGE_LAYOUT_DEPTH_STENCIL_READ_ONLY_OPTIMAL);
    UpdateDescriptorSet(4, *vkDevice, RenderData::ssaoRenderData.cameraUniformBuffers, RenderData::ssaoRenderData.descriptorSets, sizeof(CameraProjection), VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER);

    // SSAO Blur
    std::vector<VkDescriptorSetLayoutBinding> SSAOBlurBindings =
    {
        CreateDescriptorBinding(0, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, VK_SHADER_STAGE_FRAGMENT_BIT)
    };
    SSAOBlurDescriptorLayout = std::make_unique<DescriptorLayout>(*vkDevice, SSAOBlurBindings);
    DesciptorSetAllocate SSAOBluruboDescriptorSetAllocate(*vkDevice, descriptorPool->GetDescriptorPool(), SSAOBlurDescriptorLayout->GetDescriptorSetLayout(), RenderData::SSAOBlur.descriptorSets, 3);
    UpdateDescriptorSet(0, *vkDevice, RenderData::SSAOBlur.descriptorSets, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, SSAOImage->GetImageView(), noiseTextureImage->GetSampler(), VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);

    // Light
    std::vector<VkDescriptorSetLayoutBinding> LightPassBinding =
    {
        CreateDescriptorBinding(0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_FRAGMENT_BIT | VK_SHADER_STAGE_VERTEX_BIT),
        CreateDescriptorBinding(1, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, VK_SHADER_STAGE_FRAGMENT_BIT),
        CreateDescriptorBinding(2, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, VK_SHADER_STAGE_FRAGMENT_BIT),
        CreateDescriptorBinding(3, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, VK_SHADER_STAGE_FRAGMENT_BIT),
        CreateDescriptorBinding(4, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_FRAGMENT_BIT | VK_SHADER_STAGE_VERTEX_BIT),
        CreateDescriptorBinding(5, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, VK_SHADER_STAGE_FRAGMENT_BIT),
        CreateDescriptorBinding(6, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, VK_SHADER_STAGE_FRAGMENT_BIT)
    };
    LightingPassDescriptorLayout = std::make_unique<DescriptorLayout>(*vkDevice, LightPassBinding);
    DesciptorSetAllocate LightinguboDescriptorSetAllocate(*vkDevice, descriptorPool->GetDescriptorPool(), LightingPassDescriptorLayout->GetDescriptorSetLayout(), RenderData::Lighting.descriptorSets, 3);
    UpdateDescriptorSet(0, *vkDevice, RenderData::Lighting.UniformBuffers, RenderData::Lighting.descriptorSets, sizeof(Light), VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER);
    UpdateDescriptorSet(1, *vkDevice, RenderData::Lighting.descriptorSets, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, positionImage->GetImageView(), noiseTextureImage->GetSampler(), VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);
    UpdateDescriptorSet(2, *vkDevice, RenderData::Lighting.descriptorSets, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, normalImage->GetImageView(), noiseTextureImage->GetSampler(), VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);
    UpdateDescriptorSet(3, *vkDevice, RenderData::Lighting.descriptorSets, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, ShadowMapImage->GetImageView(), noiseTextureImage->GetSampler(), VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);
    UpdateDescriptorSet(4, *vkDevice, RenderData::ssaoRenderData.cameraUniformBuffers, RenderData::Lighting.descriptorSets, sizeof(CameraProjection), VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER);
    UpdateDescriptorSet(5, *vkDevice, RenderData::Lighting.descriptorSets, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, RenderData::depthResources.DepthImageView, noiseTextureImage->GetSampler(), VK_IMAGE_LAYOUT_DEPTH_STENCIL_READ_ONLY_OPTIMAL);
    UpdateDescriptorSet(6, *vkDevice, RenderData::Lighting.descriptorSets, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, SSAOBlurRenderImage->GetImageView(), noiseTextureImage->GetSampler(), VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);

    // Display
    std::vector<VkDescriptorSetLayoutBinding> DisplayBindings =
    {
        CreateDescriptorBinding(0, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, VK_SHADER_STAGE_FRAGMENT_BIT),
        CreateDescriptorBinding(1, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, VK_SHADER_STAGE_FRAGMENT_BIT),
        CreateDescriptorBinding(2, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, VK_SHADER_STAGE_FRAGMENT_BIT),
        CreateDescriptorBinding(3, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_FRAGMENT_BIT),
        CreateDescriptorBinding(4, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, VK_SHADER_STAGE_FRAGMENT_BIT)
    };
    DisplayDescriptorLayout = std::make_unique<DescriptorLayout>(*vkDevice, DisplayBindings);
    DesciptorSetAllocate DisplayuboDescriptorSetAllocate(*vkDevice, descriptorPool->GetDescriptorPool(), DisplayDescriptorLayout->GetDescriptorSetLayout(), RenderData::DisplayQuad.descriptorSets, 3);
    UpdateDescriptorSet(0, *vkDevice, RenderData::DisplayQuad.descriptorSets, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, SSAOBlurRenderImage->GetImageView(), noiseTextureImage->GetSampler(), VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);
    UpdateDescriptorSet(1, *vkDevice, RenderData::DisplayQuad.descriptorSets, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, SponzaSceneRenderedImage->GetImageView(), noiseTextureImage->GetSampler(), VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);
    UpdateDescriptorSet(2, *vkDevice, RenderData::DisplayQuad.descriptorSets, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, SSAOImage->GetImageView(), noiseTextureImage->GetSampler(), VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);
    UpdateDescriptorSet(3, *vkDevice, RenderData::DisplayQuad.UniformBuffers, RenderData::DisplayQuad.descriptorSets, sizeof(RenderPresentSettings), VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER);
    UpdateDescriptorSet(4, *vkDevice, RenderData::DisplayQuad.descriptorSets, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, LightingPassImage->GetImageView(), noiseTextureImage->GetSampler(), VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);
}

/* Init vulkan using instance -> connection between our application vulkan library */
void Application::create_instance()
{
    // if the validation layers are on but the they were not found
    if(validationLayers.enableValidationLayers && !validationLayers.ValidationLayersSupport()){
        throw std::runtime_error("Validation layers not available.");
    }

    VkInstanceCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    
    auto extensions = Window.GetRequiredExtensions();
    createInfo.enabledExtensionCount = static_cast<uint32_t>(extensions.size());
    createInfo.ppEnabledExtensionNames = extensions.data();

    if(validationLayers.enableValidationLayers){
        createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.validationLayers.size());
        createInfo.ppEnabledLayerNames = validationLayers.validationLayers.data();
    } else {
        createInfo.enabledLayerCount = 0;
        createInfo.pNext = nullptr;
    }
    
    if(vkCreateInstance(&createInfo, nullptr, &instance) != VK_SUCCESS)
    {
        throw std::runtime_error("failed to create a vulkan instance.");
    }
}