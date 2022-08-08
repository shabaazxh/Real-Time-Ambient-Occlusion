#include "ImGui.h"


// Init ImGui for Vulkan : implemented simplified approach following ImGui vulkan example 
void UI::InitImGui(VkDescriptorPool ImGuiDescriptorPool, VkRenderPass renderPass, VkCommandPool commandPool) {

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;

    io.IniFilename = NULL;

    ImGui_ImplGlfw_InitForVulkan(windowRef.GetWindow(), true);

    QueueFamilyIndices indices = deviceRef.FindQueueFamilies(deviceRef.GetPhysicalDevice(), windowRef.GetSurface());

    ImGui_ImplVulkan_InitInfo initInfo = {};
    initInfo.Instance = instance;
    initInfo.PhysicalDevice = deviceRef.GetPhysicalDevice();
    initInfo.Device = deviceRef.GetDevice();
    initInfo.Queue = deviceRef.GetGraphicsQueue();
    initInfo.QueueFamily = indices.graphicsFamily.value();
    initInfo.DescriptorPool = ImGuiDescriptorPool;
    initInfo.MinImageCount = 2;
    initInfo.ImageCount = 2;
    initInfo.Subpass = 0;
    initInfo.MSAASamples = VK_SAMPLE_COUNT_1_BIT;
    ImGui_ImplVulkan_Init(&initInfo, renderPass);

    ImGui::StyleColorsDark();

    VkCommandBuffer commandBuffer = BeginSingleTimeCommands(deviceRef, commandPool);
    ImGui_ImplVulkan_CreateFontsTexture(commandBuffer);
    EndSingleTimeCommands(deviceRef, commandBuffer, commandPool);
    ImGui_ImplVulkan_DestroyFontUploadObjects();
}

void UI::drawImGui(std::vector<VkFramebuffer> swapChainFramebuffers, VkExtent2D extent,  uint32_t currentFrame, uint32_t imageIndex) {
        
    ImGui_ImplVulkan_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
    
    static bool AOSettingsWindow = true;

    if(AOSettingsWindow)
    {
        ImGui::Begin("AOSettings: ", &AOSettingsWindow, 0);
        //ImGui::Text("Frametime: %f", 1000.0f / ImGui::GetIO().Framerate);
        ImGui::Text("WSAD keys to move camera");
        ImGui::Text("L to lock camera");
        ImGui::Text("U to unlock camera");
        ImGui::Text("Esc to exit application.");

        switch(Settings::AOStateController)
        {
            case Settings::AOState::SSAO:
            {
                ImGui::SliderInt("Samples", &SSAOuboController::samples, 0, 64);
                ImGui::SliderFloat("Radius", &SSAOuboController::radius, 0, 2.0);
                break;
            }

            case Settings::AOState::HBAO:
            {
                ImGui::SliderFloat("Radius", &SSAOuboController::radius, 0, 2.0);
                ImGui::SliderInt("SampleDirections", &SSAOuboController::hbaoSampleDirection, 0, 20.0);
                ImGui::SliderFloat("Steps", &SSAOuboController::hbaoSteps, 0, 0.08);
                ImGui::SliderInt("Number of Steps", &SSAOuboController::hbaoNumberOfSteps, 0, 20.0);
                ImGui::SliderFloat("AmbientLight", &SSAOuboController::hbaoAmbientLightLevel, 0, 5.0);
                break;
            }

            case Settings::AOState::AAO:
            {
                ImGui::SliderFloat("Radius", &SSAOuboController::radius, 0, 2.0);
                ImGui::SliderInt("Samples", &SSAOuboController::samples, 0, 64);
                ImGui::SliderFloat("Sigma", &SSAOuboController::alchemySigma, 0, 2.0);
                ImGui::SliderFloat("Kappa", &SSAOuboController::alchemyKappa, 0, 5.0);
                break;
            }
        }
        
        if(ImGui::RadioButton("Crytek SSAO", Settings::AOStateController == Settings::AOState::SSAO)) { Settings::AOStateController = Settings::AOState::SSAO; ImGui::SameLine(); }
        if(ImGui::RadioButton("HBAO", Settings::AOStateController == Settings::AOState::HBAO)) { Settings::AOStateController = Settings::AOState::HBAO; ImGui::SameLine();}
        if(ImGui::RadioButton("AAO", Settings::AOStateController == Settings::AOState::AAO)) { Settings::AOStateController = Settings::AOState::AAO; ImGui::SameLine();}
        ImGui::Checkbox("Blur", &RenderPresentSettingsController::enableBlur);
        ImGui::Checkbox("Light", &RenderPresentSettingsController::enableLight);
        ImGui::End();
    }

    ImGui::Render();
    ImDrawData* main_draw_data = ImGui::GetDrawData();

    VkCommandBufferBeginInfo beginInfo = {};
    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

    if(vkBeginCommandBuffer(RenderData::ImGuiData.ImGuiCmds[currentFrame], &beginInfo) != VK_SUCCESS){
        throw std::runtime_error("failed to begin recording ImGui commands");
    }

    VkRenderPassBeginInfo renderPassInfo = {};
    renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
    renderPassInfo.renderPass = RenderData::ImGuiData.renderpass;
    renderPassInfo.framebuffer = swapChainFramebuffers[imageIndex];
    renderPassInfo.renderArea.offset = {0, 0};
    renderPassInfo.renderArea.extent = extent;

    std::array<VkClearValue,2> clearValues{};
    clearValues[0].color = {{0.0f, 0.0f, 0.0f, 1.0f}};
    clearValues[1].depthStencil = {1.0f, 0};

    renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
    renderPassInfo.pClearValues = clearValues.data();

    vkCmdBeginRenderPass(RenderData::ImGuiData.ImGuiCmds[currentFrame], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);
    ImGui_ImplVulkan_RenderDrawData(main_draw_data, RenderData::ImGuiData.ImGuiCmds[currentFrame]);
    vkCmdEndRenderPass(RenderData::ImGuiData.ImGuiCmds[currentFrame]);

    if(vkEndCommandBuffer(RenderData::ImGuiData.ImGuiCmds[currentFrame]) != VK_SUCCESS){
        throw std::runtime_error("failed to record ImGui commands");
    }
}