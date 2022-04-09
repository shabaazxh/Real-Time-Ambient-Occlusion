#pragma once

#include <vulkan/vulkan.h>
#define GLFW_INCLUDE_VULKAN
#include <vector>
#include "../libs/imgui/imgui.h"
#include "../libs/imgui/imgui_impl_vulkan.h"
#include "../libs/imgui/imgui_impl_glfw.h"

#include "../Device/Device.h"
#include "../Window/Window.h"
#include "../Commands/Commandbuffer.h"
#include "../Renderer/RenderData.h"
#include "../Structs/Shaders.h"  
  
static std::vector<VkDescriptorPoolSize> ImGuiPoolSizes = {
    {VK_DESCRIPTOR_TYPE_SAMPLER,                1000},
    {VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1000},
    {VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE,          1000},
    {VK_DESCRIPTOR_TYPE_STORAGE_IMAGE,          1000},
    {VK_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER,   1000},
    {VK_DESCRIPTOR_TYPE_STORAGE_TEXEL_BUFFER,   1000},
    {VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,         1000},
    {VK_DESCRIPTOR_TYPE_STORAGE_BUFFER,         1000},
    {VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC, 1000},
    {VK_DESCRIPTOR_TYPE_STORAGE_BUFFER_DYNAMIC, 1000},
    {VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT,       1000}
};


class UI {

    public:
    UI(VkInstance instance, Device& deviceRef, Window& windowRef) : instance{instance}, deviceRef{deviceRef}, windowRef{windowRef} {}
    void InitImGui(VkDescriptorPool ImGuiDescriptorPool, VkRenderPass renderPass, VkCommandPool commandPool);
    void drawImGui(std::vector<VkFramebuffer> swapChainFramebuffers, VkExtent2D extent,  uint32_t currentFrame, uint32_t imageIndex);
    private:
    VkInstance instance;
    Device& deviceRef;
    Window& windowRef;


};
