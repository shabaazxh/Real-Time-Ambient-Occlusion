#include "Renderer.h"



void Renderer::DestroyRendererResources()
{

    for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
    {
        imageAvailableSemaphore[i].DestroySemaphore(deviceRef.GetDevice());
        renderFinishedSemaphore[i].DestroySemaphore(deviceRef.GetDevice());
    }

    for(auto& fence: inFlightFence){
        vkDestroyFence(deviceRef.GetDevice(), fence, nullptr);
    }
}

// Create semaphore objects to synchronize the GPU
void Renderer::CreateSync() {
    imageAvailableSemaphore.resize(MAX_FRAMES_IN_FLIGHT);
    renderFinishedSemaphore.resize(MAX_FRAMES_IN_FLIGHT);

    for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
    {
        imageAvailableSemaphore[i].CreateSemaphore(deviceRef.GetDevice());
        renderFinishedSemaphore[i].CreateSemaphore(deviceRef.GetDevice());
    }
    
    CreateFence();

}

// Create fence objects for CPU synchronization 
void Renderer::CreateFence() {
    inFlightFence.resize(MAX_FRAMES_IN_FLIGHT);

    VkFenceCreateInfo fenceInfo = {};
    fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
    fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

    for(size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++){

        if(vkCreateFence(deviceRef.GetDevice(), &fenceInfo, nullptr, &inFlightFence[i]) != VK_SUCCESS){
            throw std::runtime_error("failed to create fence.");
        }
    }

}

void Renderer::UpdateCamera()
{
    float deltaTime = 0.0f;
    float lastFrame = 0.0f;
    float currentFrame = glfwGetTime();
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;
    glm::vec3 right = glm::normalize(glm::cross(CameraController.GetCameraFront(), CameraController.GetCameraUp()));
    float CameraSpeed = 0.00025f * deltaTime;

    if(glfwGetKey(windowRef.GetWindow(), GLFW_KEY_W) == GLFW_PRESS){
        CameraController.SetCameraPos(CameraController.GetCameraPos() += CameraSpeed * CameraController.GetCameraFront());
    }

    if(glfwGetKey(windowRef.GetWindow(), GLFW_KEY_S) == GLFW_PRESS){
        CameraController.SetCameraPos(CameraController.GetCameraPos() -= CameraSpeed * CameraController.GetCameraFront());
    }

    if(glfwGetKey(windowRef.GetWindow(), GLFW_KEY_D) == GLFW_PRESS){
        CameraController.SetCameraPos(CameraController.GetCameraPos() += glm::normalize(glm::cross(CameraController.GetCameraFront(), CameraController.GetCameraUp())) * CameraSpeed);
    }

    if(glfwGetKey(windowRef.GetWindow(), GLFW_KEY_A) == GLFW_PRESS){
        CameraController.SetCameraPos(CameraController.GetCameraPos() -= glm::normalize(glm::cross(CameraController.GetCameraFront(), CameraController.GetCameraUp())) * CameraSpeed);
    }

    if(glfwGetKey(windowRef.GetWindow(), GLFW_KEY_SPACE) == GLFW_PRESS){
        glm::vec3 currentPosition = CameraController.GetCameraPos();
        currentPosition.y += CameraSpeed;
        CameraController.SetCameraPos(currentPosition);
    }

    if(glfwGetKey(windowRef.GetWindow(), GLFW_KEY_R) == GLFW_PRESS){
        glm::vec3 currentPosition = CameraController.GetCameraPos();
        currentPosition.y -= CameraSpeed;
        CameraController.SetCameraPos(currentPosition);
    }

    if(glfwGetKey(windowRef.GetWindow(), GLFW_KEY_L) == GLFW_PRESS){
        
        if(CameraController.GetCameraLock() == false){

            CameraController.SetCameraLock(true);
            windowRef.LockWindowCamera(true);
        } 
    }

    if(glfwGetKey(windowRef.GetWindow(), GLFW_KEY_U) == GLFW_PRESS){
        
        if(CameraController.GetCameraLock() == true) {
            CameraController.SetCameraLock(false);
            windowRef.LockWindowCamera(false);
        }
    }

    if(glfwGetKey(windowRef.GetWindow(), GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(windowRef.GetWindow(), true);
    }
}
// Data that needs to be updated every frame is set to be updated here
void Renderer::UpdateUniforms(uint32_t currentImage) {

    static auto startTime = std::chrono::high_resolution_clock::now();

    auto currentTime = std::chrono::high_resolution_clock::now();
    float time = std::chrono::duration<float, std::chrono::seconds::period>(currentTime - startTime).count();

    CameraProjection camera{};
    camera.model = glm::mat4(1.0f);
    camera.model = glm::translate(camera.model, glm::vec3(0.0f, 0.0f, 0.0f));
    camera.view = glm::lookAt(CameraController.GetCameraPos(), CameraController.GetCameraPos() + CameraController.GetCameraFront(), CameraController.GetCameraUp());
    camera.proj = glm::perspective(glm::radians(45.0f), swapChainRef.GetSwapChainExtent().width / (float) swapChainRef.GetSwapChainExtent().height, 0.1f, 1000.0f);
    camera.proj[1][1] *= -1;

    //(Joey De Vries, 2020) Original implementation uses a hemisphere. My changes make this a sphere kernel
    std::uniform_real_distribution<float> randomFloats(0.0, 1.0);
    std::default_random_engine generator;
    std::vector<glm::vec4> ssaoKernel;
    SSAOubo ssao{};
    for(unsigned int i = 0; i < 64; i++) {
        glm::vec4 sample(
            randomFloats(generator) * 2.0 - 1.0,
            randomFloats(generator) * 2.0 - 1.0,
            randomFloats(generator) * 2.0 - 1.0, 0.0);

            sample = glm::normalize(sample);
            sample *= randomFloats(generator);

            float scale = float(i) / 64;
            scale = Lerp(0.1f, 1.0f, scale * scale);
            sample *= scale;
            ssaoKernel.push_back(sample);
    }

    for(unsigned int i = 0; i <  64; i++) {
        ssao.samples[i] = ssaoKernel[i];
    }
    ssao.radius = SSAOuboController::radius;
    ssao.sample_amount = SSAOuboController::samples;

    ssao.hbaoSampleDirection = SSAOuboController::hbaoSampleDirection;
    ssao.hbaoSteps = SSAOuboController::hbaoSteps;
    ssao.hbaoNumberOfSteps = SSAOuboController::hbaoNumberOfSteps;
    ssao.hbaoAmbientLightLevel = SSAOuboController::hbaoAmbientLightLevel;

    ssao.alchemySigma = SSAOuboController::alchemySigma;
    ssao.alchemyKappa = SSAOuboController::alchemyKappa;

    ssao.time = glfwGetTime();

    Light Light{};
    //Light.LightPosition = glm::vec4(0.0f, 5.0f, 2.0f, 1.0f);
    Light.LightPosition = glm::vec4(0.0f, 4.0f, 2.0f, 0.0f);
    Light.LightColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
    Light.ObjectColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
    Light.LightPosition.x = 1.0f + sin(glfwGetTime()) * 2.0f;
    //Light.LightPosition.y = sin(glfwGetTime() / 2.0f) * 1.0f;

    //glm::mat4 LightProjection = glm::ortho(-10.0f, 10.0f, 10.0f, -10.0f, 1.0f, 7.5f);
    glm::mat4 LightProjection = glm::perspective(glm::radians(45.0f), 1.0f, 1.0f, 30.0f);
    glm::mat4 LightView = glm::lookAt(glm::vec3(Light.LightPosition), glm::vec3(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    Light.LightSpaceMatrix = LightProjection * LightView;

    RenderPresentSettings presentSettings{};
    presentSettings.enableBlur = RenderPresentSettingsController::enableBlur;
    presentSettings.enableLight = RenderPresentSettingsController::enableLight;

    void* data;
    vkMapMemory(deviceRef.GetDevice(), RenderData::SponzaData.UniformMemory[currentImage], 0, sizeof(camera), 0, &data);
    memcpy(data, &camera, sizeof(camera));
    vkUnmapMemory(deviceRef.GetDevice(), RenderData::SponzaData.UniformMemory[currentImage]);

    vkMapMemory(deviceRef.GetDevice(), RenderData::ssaoRenderData.cameraUniformMemory[currentImage], 0, sizeof(camera), 0, &data);
    memcpy(data, &camera, sizeof(camera));
    vkUnmapMemory(deviceRef.GetDevice(), RenderData::ssaoRenderData.cameraUniformMemory[currentImage]);

    vkMapMemory(deviceRef.GetDevice(), RenderData::ssaoRenderData.UniformMemory[currentImage], 0, sizeof(ssao), 0, &data);
    memcpy(data, &ssao, sizeof(ssao));
    vkUnmapMemory(deviceRef.GetDevice(), RenderData::ssaoRenderData.UniformMemory[currentImage]);

    vkMapMemory(deviceRef.GetDevice(), RenderData::DisplayQuad.UniformMemory[currentImage], 0, sizeof(presentSettings), 0, &data);
    memcpy(data, &presentSettings, sizeof(presentSettings));
    vkUnmapMemory(deviceRef.GetDevice(), RenderData::DisplayQuad.UniformMemory[currentImage]);

    vkMapMemory(deviceRef.GetDevice(), RenderData::Lighting.UniformMemory[currentImage], 0, sizeof(Light), 0, &data);
    memcpy(data, &Light, sizeof(Light));
    vkUnmapMemory(deviceRef.GetDevice(), RenderData::Lighting.UniformMemory[currentImage]);
}

void Renderer::Renderframe() {
    // Wait until previous frame has finished so that the semaphores are available to use 
    vkWaitForFences(deviceRef.GetDevice(), 1, &inFlightFence[currentFrame], VK_TRUE, UINT64_MAX);

    // Acquire the next available image
    uint32_t imageIndex;
    vkAcquireNextImageKHR(deviceRef.GetDevice(), swapChainRef.GetSwapChain(), UINT64_MAX, imageAvailableSemaphore[currentFrame].GetSemaphore(), VK_NULL_HANDLE, &imageIndex);

    ImGuiUIRef.drawImGui(swapChainRef.GetSwapChainFramebuffers(), swapChainRef.GetSwapChainExtent(), currentFrame, imageIndex);
    UpdateUniforms(currentFrame);

    // Reset the Fence to an unsignaled state
    vkResetFences(deviceRef.GetDevice(), 1, &inFlightFence[currentFrame]);

    vkResetCommandBuffer(commandBuffers[currentFrame], 0);
    RecordCommandbuffer(currentFrame, imageIndex);
    
    // Submitting the command buffer
    VkSubmitInfo submitInfo = {};
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    
    VkSemaphore waitSemphores[]  = {imageAvailableSemaphore[currentFrame].GetSemaphore()};
    VkPipelineStageFlags waitStages[] = {VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};
    submitInfo.waitSemaphoreCount = 1;
    submitInfo.pWaitSemaphores = waitSemphores;
    submitInfo.pWaitDstStageMask = waitStages;
    
    VkCommandBuffer commands[] = { commandBuffers[currentFrame], RenderData::ImGuiData.ImGuiCmds[currentFrame]};
    submitInfo.commandBufferCount = 2;
    submitInfo.pCommandBuffers = commands;

    VkSemaphore signalSemaphores[] = {renderFinishedSemaphore[currentFrame].GetSemaphore()};
    submitInfo.signalSemaphoreCount = 1;
    submitInfo.pSignalSemaphores = signalSemaphores;

    // Submit commands to the graphics queue
    if(vkQueueSubmit(deviceRef.GetGraphicsQueue(), 1, &submitInfo, inFlightFence[currentFrame]) != VK_SUCCESS){
        throw std::runtime_error("failed to submit draw command buffer");
    }

    VkPresentInfoKHR presentInfo = {};
    presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
    presentInfo.waitSemaphoreCount = 1;
    presentInfo.pWaitSemaphores = signalSemaphores;
    
    VkSwapchainKHR swapChains[] = {swapChainRef.GetSwapChain()};
    presentInfo.swapchainCount = 1;
    presentInfo.pSwapchains = swapChains;
    presentInfo.pImageIndices = &imageIndex;

    // Submit to present queue for presentation
    vkQueuePresentKHR(deviceRef.GetPresentQueue(), &presentInfo);

    currentFrame = (currentFrame + 1) % MAX_FRAMES_IN_FLIGHT;
}

void Renderer::RecordCommandbuffer(uint32_t currentFrame, uint32_t imageIndex){

    VkCommandBufferBeginInfo beginInfo = {};
    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    beginInfo.flags = 0;
    beginInfo.pInheritanceInfo = nullptr;

    if(vkBeginCommandBuffer(commandBuffers[currentFrame], &beginInfo) != VK_SUCCESS){
        throw std::runtime_error("failed to begin recording command buffer!");
    }
    // --------------------------------------------------------------------------------------------
    // Shadow map 
    {
        VkRenderPassBeginInfo renderPassInfo = {};
        renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
        renderPassInfo.renderPass = RenderData::Shadow.renderpass;
        renderPassInfo.framebuffer = RenderData::Shadow.framebuffer; 
        renderPassInfo.renderArea.offset = {0,0};
        renderPassInfo.renderArea.extent = {2048, 2048};

        std::array<VkClearValue,1> clearValues{};
        clearValues[0].depthStencil = {1.0f, 0};

        renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
        renderPassInfo.pClearValues = clearValues.data();

        vkCmdBeginRenderPass(commandBuffers[currentFrame], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);
        vkCmdBindPipeline(commandBuffers[currentFrame], VK_PIPELINE_BIND_POINT_GRAPHICS, RenderData::Shadow.pipeline);
        
        VkBuffer vertexBuffers[] = {RenderData::SponzaData.vertexBuffer};
        VkDeviceSize offsets[] = {0};

        vkCmdBindVertexBuffers(commandBuffers[currentFrame], 0, 1, vertexBuffers, offsets);
        vkCmdBindDescriptorSets(commandBuffers[currentFrame], VK_PIPELINE_BIND_POINT_GRAPHICS, RenderData::Shadow.pipelineLayout, 0, 1,
        &RenderData::Lighting.descriptorSets[currentFrame], 0, nullptr);
        
        vkCmdDraw(commandBuffers[currentFrame], static_cast<uint32_t>(RenderData::SponzaData.vertexData.size()), 1, 0, 0);

        vkCmdEndRenderPass(commandBuffers[currentFrame]);
    }

     //----------------------------------------------------------------------------------------
    // Render main Sponza scene
    {
        VkRenderPassBeginInfo renderPassInfo = {};
        renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
        renderPassInfo.renderPass = RenderData::SponzaData.renderpass;
        renderPassInfo.framebuffer = RenderData::SponzaData.framebuffer; //swapChainRef.GetSwapChainFramebuffers()[imageIndex]
        renderPassInfo.renderArea.offset = {0,0};
        renderPassInfo.renderArea.extent = swapChainRef.GetSwapChainExtent();

        std::array<VkClearValue,2> clearValues{};
        clearValues[0].color = {{0.0f, 0.0f, 0.0f, 1.0f}};
        clearValues[1].depthStencil = {1.0f, 0};

        renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
        renderPassInfo.pClearValues = clearValues.data();

        vkCmdBeginRenderPass(commandBuffers[currentFrame], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);
        vkCmdBindPipeline(commandBuffers[currentFrame], VK_PIPELINE_BIND_POINT_GRAPHICS, RenderData::SponzaData.pipeline);
        
        VkBuffer vertexBuffers[] = {RenderData::SponzaData.vertexBuffer};
        VkDeviceSize offsets[] = {0};

        vkCmdBindVertexBuffers(commandBuffers[currentFrame], 0, 1, vertexBuffers, offsets);
        vkCmdBindDescriptorSets(commandBuffers[currentFrame], VK_PIPELINE_BIND_POINT_GRAPHICS, RenderData::SponzaData.pipelineLayout, 0, 1,
        &RenderData::SponzaData.descriptorSets[currentFrame], 0, nullptr);
        
        vkCmdDraw(commandBuffers[currentFrame], static_cast<uint32_t>(RenderData::SponzaData.vertexData.size()), 1, 0, 0);

        vkCmdEndRenderPass(commandBuffers[currentFrame]);
    }

    //---------------------------------------------------------------------------------
    // Render G-buffer
    {
        VkRenderPassBeginInfo renderPassInfo = {};
        renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
        renderPassInfo.renderPass = RenderData::GbufferRenderData.renderpass;
        renderPassInfo.framebuffer = RenderData::GbufferRenderData.framebuffer;
        renderPassInfo.renderArea.offset = {0,0};
        renderPassInfo.renderArea.extent = swapChainRef.GetSwapChainExtent();

        std::array<VkClearValue, 3> clearValues{};
        clearValues[0].color = {{0.0f, 0.0f, 0.0f, 1.0f}};
        clearValues[1].color = {{0.0f, 0.0f, 0.0f, 1.0f}};
        clearValues[2].depthStencil = {1.0f, 0};

        renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
        renderPassInfo.pClearValues = clearValues.data();

        vkCmdBeginRenderPass(commandBuffers[currentFrame], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);
        vkCmdBindPipeline(commandBuffers[currentFrame], VK_PIPELINE_BIND_POINT_GRAPHICS, RenderData::GbufferRenderData.pipeline);
        
        VkBuffer vertexBuffers[] = {RenderData::SponzaData.vertexBuffer};
        VkDeviceSize offsets[] = {0};

        vkCmdBindVertexBuffers(commandBuffers[currentFrame], 0, 1, vertexBuffers, offsets);
        vkCmdBindDescriptorSets(commandBuffers[currentFrame], VK_PIPELINE_BIND_POINT_GRAPHICS, RenderData::GbufferRenderData.pipelineLayout, 0, 1,
        &RenderData::SponzaData.descriptorSets[currentFrame], 0, nullptr);
        
        vkCmdDraw(commandBuffers[currentFrame], static_cast<uint32_t>(RenderData::SponzaData.vertexData.size()), 1, 0, 0);

        vkCmdEndRenderPass(commandBuffers[currentFrame]);
    }

    //---------------------------------------------------------------------------------
    // SSAO Render pass
    {
        VkRenderPassBeginInfo renderPassInfo = {};
        renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
        renderPassInfo.renderPass = RenderData::ssaoRenderData.renderpass;
        renderPassInfo.framebuffer = RenderData::ssaoRenderData.framebuffer;
        renderPassInfo.renderArea.offset = {0,0};
        renderPassInfo.renderArea.extent = swapChainRef.GetSwapChainExtent();

        std::array<VkClearValue, 2> clearValues{};
        clearValues[0].color = {{0.0f, 0.0f, 0.0f, 1.0f}};
        clearValues[1].depthStencil = {1.0f, 0};

        renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
        renderPassInfo.pClearValues = clearValues.data();

        vkCmdBeginRenderPass(commandBuffers[currentFrame], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);
        // Switch depending on the current AO method selected
        if(Settings::AOStateController == Settings::AOState::SSAO)
        {
            vkCmdBindPipeline(commandBuffers[currentFrame], VK_PIPELINE_BIND_POINT_GRAPHICS, RenderData::ssaoRenderData.ssaoPipeline);
        }
        
        if(Settings::AOStateController == Settings::AOState::HBAO)
        {
            vkCmdBindPipeline(commandBuffers[currentFrame], VK_PIPELINE_BIND_POINT_GRAPHICS, RenderData::ssaoRenderData.hbaoPipeline);
        }

        if(Settings::AOStateController == Settings::AOState::AAO)
        {
            vkCmdBindPipeline(commandBuffers[currentFrame], VK_PIPELINE_BIND_POINT_GRAPHICS, RenderData::ssaoRenderData.alchchemyPipeline);
        }
        
        VkBuffer vertexBuffers[] = {RenderData::ssaoRenderData.vertexBuffer};
        VkDeviceSize offsets[] = {0};

        vkCmdBindVertexBuffers(commandBuffers[currentFrame], 0, 1, vertexBuffers, offsets);
        vkCmdBindDescriptorSets(commandBuffers[currentFrame], VK_PIPELINE_BIND_POINT_GRAPHICS, RenderData::ssaoRenderData.pipelineLayout, 0, 1,
        &RenderData::ssaoRenderData.descriptorSets[currentFrame], 0, nullptr);
        vkCmdDraw(commandBuffers[currentFrame], static_cast<uint32_t>(RenderData::ssaoRenderData.vertexData.size()), 1, 0, 0);

        vkCmdEndRenderPass(commandBuffers[currentFrame]);
    }

    //---------------------------------------------------------------------------------------
    // Blur SSAO pass
    {
        VkRenderPassBeginInfo renderPassInfo = {};
        renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
        renderPassInfo.renderPass = RenderData::SSAOBlur.renderpass;
        renderPassInfo.framebuffer = RenderData::SSAOBlur.framebuffer;
        renderPassInfo.renderArea.offset = {0,0};
        renderPassInfo.renderArea.extent = swapChainRef.GetSwapChainExtent();

        std::array<VkClearValue,2> clearValues{};
        clearValues[0].color = {{0.0f, 0.0f, 0.0f, 1.0f}};
        clearValues[1].depthStencil = {1.0f, 0};

        renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
        renderPassInfo.pClearValues = clearValues.data();

        vkCmdBeginRenderPass(commandBuffers[currentFrame], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);
        vkCmdBindPipeline(commandBuffers[currentFrame], VK_PIPELINE_BIND_POINT_GRAPHICS, RenderData::SSAOBlur.pipeline);

        VkBuffer vertexBuffers[] = {RenderData::DisplayQuad.vertexBuffer};
        VkDeviceSize offsets[] = {0};

        vkCmdBindVertexBuffers(commandBuffers[currentFrame], 0, 1, vertexBuffers, offsets);
        vkCmdBindDescriptorSets(commandBuffers[currentFrame], VK_PIPELINE_BIND_POINT_GRAPHICS, RenderData::SSAOBlur.pipelineLayout, 0, 1,
        &RenderData::SSAOBlur.descriptorSets[currentFrame], 0, nullptr);
        vkCmdDraw(commandBuffers[currentFrame], static_cast<uint32_t>(RenderData::DisplayQuad.vertexData.size()), 1, 0, 0);

        vkCmdEndRenderPass(commandBuffers[currentFrame]);
    }
    // Lighting configuration set up to provide easy user extensibility 
    //---------------------------------------------------------------------------------------
    // Lighting
    {
        VkRenderPassBeginInfo renderPassInfo = {};
        renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
        renderPassInfo.renderPass = RenderData::Lighting.renderpass;
        renderPassInfo.framebuffer = RenderData::Lighting.framebuffer;
        renderPassInfo.renderArea.offset = {0,0};
        renderPassInfo.renderArea.extent = swapChainRef.GetSwapChainExtent();

        std::array<VkClearValue,2> clearValues{};
        clearValues[0].color = {{0.0f, 0.0f, 0.0f, 1.0f}};
        clearValues[1].depthStencil = {1.0f, 0};

        renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
        renderPassInfo.pClearValues = clearValues.data();

        vkCmdBeginRenderPass(commandBuffers[currentFrame], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);
        vkCmdBindPipeline(commandBuffers[currentFrame], VK_PIPELINE_BIND_POINT_GRAPHICS, RenderData::Lighting.pipeline);

        VkBuffer vertexBuffers[] = {RenderData::DisplayQuad.vertexBuffer};
        VkDeviceSize offsets[] = {0};

        vkCmdBindVertexBuffers(commandBuffers[currentFrame], 0, 1, vertexBuffers, offsets);
        vkCmdBindDescriptorSets(commandBuffers[currentFrame], VK_PIPELINE_BIND_POINT_GRAPHICS, RenderData::Lighting.pipelineLayout, 0, 1,
        &RenderData::Lighting.descriptorSets[currentFrame], 0, nullptr);
        vkCmdDraw(commandBuffers[currentFrame], static_cast<uint32_t>(RenderData::DisplayQuad.vertexData.size()), 1, 0, 0);

        vkCmdEndRenderPass(commandBuffers[currentFrame]);
    }

    //----------------------------------------------------------------------------------------
    // Swapchain present
    {
        VkRenderPassBeginInfo renderPassInfo = {};
        renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
        renderPassInfo.renderPass = swapChainRef.GetSwapChainRenderpass();
        renderPassInfo.framebuffer = swapChainRef.GetSwapChainFramebuffers()[imageIndex];
        renderPassInfo.renderArea.offset = {0,0};
        renderPassInfo.renderArea.extent = swapChainRef.GetSwapChainExtent();

        std::array<VkClearValue,2> clearValues{};
        clearValues[0].color = {{0.0f, 0.0f, 0.0f, 1.0f}};
        clearValues[1].depthStencil = {1.0f, 0};

        renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
        renderPassInfo.pClearValues = clearValues.data();

        vkCmdBeginRenderPass(commandBuffers[currentFrame], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);
        vkCmdBindPipeline(commandBuffers[currentFrame], VK_PIPELINE_BIND_POINT_GRAPHICS, RenderData::DisplayQuad.pipeline);
        
        VkBuffer vertexBuffers[] = {RenderData::DisplayQuad.vertexBuffer};
        VkDeviceSize offsets[] = {0};

        vkCmdBindVertexBuffers(commandBuffers[currentFrame], 0, 1, vertexBuffers, offsets);
        vkCmdBindDescriptorSets(commandBuffers[currentFrame], VK_PIPELINE_BIND_POINT_GRAPHICS, RenderData::DisplayQuad.pipelineLayout, 0, 1,
        &RenderData::DisplayQuad.descriptorSets[currentFrame], 0, nullptr);
        
        vkCmdDraw(commandBuffers[currentFrame], static_cast<uint32_t>(RenderData::DisplayQuad.vertexData.size()), 1, 0, 0);

        vkCmdEndRenderPass(commandBuffers[currentFrame]);
    }

    if(vkEndCommandBuffer(commandBuffers[currentFrame]) != VK_SUCCESS){
        throw std::runtime_error("failed to recordd command buffer.");
    }
}