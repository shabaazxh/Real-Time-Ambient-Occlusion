
#include <vulkan/vulkan.h>
#define GLFW_INCLUDE_VULKAN
#include <iostream>
#include <GLFW/glfw3.h>
#include "../libs/glm/glm.hpp"

//std
#include <iostream>
#include <stdexcept>
#include <vector>
#include <cstdint>
#include <memory>

#include "../Device/Device.h"
#include "../Validation.h"
#include "../Window/Window.h"
#include "../Swapchain/Swapchain.h"
#include "../Pipelines/Pipelines.h"
#include "../Renderpass/Renderpass.h"
#include "../Commands/CommandPool.h"
#include "../Commands/Commandbuffer.h"
#include "../Renderer/Renderer.h"
#include "../Resources/Buffer.h"
#include "../Renderer/RenderData.h"
#include "../Descriptors/DescriptorLayout.h"
#include "../Descriptors/DescriptorPool.h"
#include "../Model/Model.h"
#include "../ImGui/ImGui.h"
#include "../Camera/Camera.h"

class Application {

public:

    Application(bool ValidationLayers){
        validationLayers.enableValidationLayers = ValidationLayers;
    }

    void run();
   
private:

    void init_window();

    void setup_renderpass();

    void setup_image_resources();

    void setup_framebuffers();

    void setup_pipelines();

    void init_vulkan();

    void mainLoop() ;

    void cleanup_swapchain();

    void cleanup_render_resources();

    void recreate_swapchain();

    void cleanup_resources();

    void setup_descriptor_sets();

    void create_instance();

    void GenerateSSAONoise(Image& image, VkFormat format, std::vector<glm::vec4>& ssaoNoise, VkCommandPool commandPool, Device& deviceRef);


private:
    std::string path_for_assets;

    VkInstance instance;
    Validation validationLayers;
    Primitives primitives;

    Window Window;
    std::unique_ptr<SwapChain> swapChain;
    std::unique_ptr<Device> vkDevice = std::make_unique<Device>();

    std::unique_ptr<Pipeline> SponzaGraphicsPipline;
    std::unique_ptr<Pipeline> GBufferPipeline;
    std::unique_ptr<Pipeline> SSAOPipeline;
    std::unique_ptr<Pipeline> HBAOPipeline;
    std::unique_ptr<Pipeline> AlchemyPipeline;
    std::unique_ptr<Pipeline> SSAOBlurPipeline;
    std::unique_ptr<Pipeline> LightPassPipeline;
    std::unique_ptr<Pipeline> DisplayQuad;
    std::unique_ptr<Pipeline> ShadowPipeline;
   
    std::unique_ptr<Renderpass> SwapChainRenderPass;
    std::unique_ptr<Renderpass> GBufferRenderPass;
    std::unique_ptr<Renderpass> SSAORenderpass;
    std::unique_ptr<Renderpass> SponzaSceneRenderpass;
    std::unique_ptr<Renderpass> ImGuiRenderpass;
    std::unique_ptr<Renderpass> SSAOBlurRenderpass;
    std::unique_ptr<Renderpass> LightingPassRenderpass;
    std::unique_ptr<Renderpass> ShadowMapRenderpass;

    std::unique_ptr<Buffer> ModelBuffer;
    std::unique_ptr<Buffer> SSAOBuffer;
    std::unique_ptr<Buffer> SSAOQuadBuffer;
    std::unique_ptr<Buffer> DisplayQuadBuffer;
    std::unique_ptr<Buffer> LightBuffer;

    std::unique_ptr<Framebuffer> GbufferFramebuffer;
    std::unique_ptr<Framebuffer> SSAOFramebuffer;
    std::unique_ptr<Framebuffer> SponzaSceneFramebuffer;
    std::unique_ptr<Framebuffer> SSAOBlurAOFramebuffer;
    std::unique_ptr<Framebuffer> LightingPassFramebuffer;
    std::unique_ptr<Framebuffer> ShadowMapFramebuffer;

    std::unique_ptr<DescriptorPool> descriptorPool;
    std::unique_ptr<DescriptorPool> ImGuiDescriptorPool;
    std::unique_ptr<DescriptorLayout> uboDescriptorLayout;
    std::unique_ptr<DescriptorLayout> SSAOuboDescriptorLayout;
    std::unique_ptr<DescriptorLayout> SSAOBlurDescriptorLayout;
    std::unique_ptr<DescriptorLayout> DisplayDescriptorLayout;
    std::unique_ptr<DescriptorLayout> LightingPassDescriptorLayout;
    std::unique_ptr<CommandPool> commandPool;
    std::unique_ptr<Renderer> Render;

    std::unique_ptr<Image> positionImage;
    std::unique_ptr<Image> normalImage;
    std::unique_ptr<Image> SSAOImage;
    std::unique_ptr<Image> noiseTextureImage;
    std::unique_ptr<Image> depthSampleImage;
    std::unique_ptr<Image> SponzaSceneRenderedImage;
    std::unique_ptr<Image> SSAOBlurRenderImage;
    std::unique_ptr<Image> LightingPassImage;
    std::unique_ptr<Image> ShadowMapImage;

    std::unique_ptr<UI> ImGuiUI;   
    std::unique_ptr<Camera> playerCamera;
    std::vector<glm::vec4> ssaoNoise;

};