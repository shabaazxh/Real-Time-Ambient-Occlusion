
#include <vulkan/vulkan.h>
#include <vector>
#include <chrono>
#include <array>
#include <random>
#include <memory>

#include "../libs/glm/glm.hpp"
#include "../libs/glm/gtc/matrix_transform.hpp"
#include "../libs/glm/gtx/hash.hpp"
#include "../Commands/Commandbuffer.h"
#include "../Swapchain/Swapchain.h"
#include "../Window/Window.h"
#include "../Resources/Buffer.h"
#include "Semaphore.h"
#include "RenderData.h"
#include "../ImGui/ImGui.h"
#include "../Structs/Shaders.h"
#include "../Camera/Camera.h"

class Renderer {
    public:
    Renderer(Device& deviceRef, SwapChain& swapChainRef, UI& ImGuiUIRef, Window& windowRef, std::vector<VkCommandBuffer> commandBuffers) : 
        deviceRef{deviceRef}, swapChainRef{swapChainRef}, ImGuiUIRef{ImGuiUIRef}, windowRef{windowRef}, commandBuffers{commandBuffers} {
            MouseControls = &CameraController;
        }
    void CreateSync();
    void Renderframe();
    void UpdateUniforms(uint32_t currentImage);
    void RecordCommandbuffer(uint32_t currentFrame, uint32_t imageIndex);
    void UpdateCamera();
    void DestroyRendererResources();
    
    
    private:
    void CreateFence();

    private:

    Device& deviceRef;
    UI& ImGuiUIRef;
    Window& windowRef;
    Camera CameraController;
    std::vector<VkCommandBuffer> commandBuffers;
    std::vector<Semaphore> imageAvailableSemaphore;
    std::vector<Semaphore> renderFinishedSemaphore;
    std::vector<VkFence> inFlightFence;
    SwapChain& swapChainRef;

    const int MAX_FRAMES_IN_FLIGHT = 2;
    uint32_t currentFrame = 0;
};