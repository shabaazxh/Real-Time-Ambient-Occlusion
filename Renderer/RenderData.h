#pragma once

#include <vulkan/vulkan.h>
#include <vector>
#include <map>
#include <string>
#include <memory>
#include "../Structs/Shaders.h"

struct Global {
     VkInstance instance;
     VkCommandPool commandPool;
     std::vector<VkCommandBuffer> RenderingCommands;
};

 struct RenderData {

    struct Quad {
         std::vector<Vertex> vertexData;
         VkBuffer vertexBuffer;

         std::vector<VkBuffer> UniformBuffers;
         std::vector<VkDeviceMemory> UniformMemory;

         std::vector<VkDescriptorSet> descriptorSets;
         VkPipeline pipeline;
         VkPipelineLayout pipelineLayout;
         VkFramebuffer framebuffer;
         VkRenderPass renderpass;
    };

    struct ModelData {
         std::vector<Vertex> vertexData;
         VkBuffer vertexBuffer;

         std::vector<VkBuffer> UniformBuffers;
         std::vector<VkDeviceMemory> UniformMemory;
         std::vector<VkDescriptorSet> descriptorSets;

         VkPipeline pipeline;
         VkPipelineLayout pipelineLayout;
         VkFramebuffer framebuffer;
         VkRenderPass renderpass;
    };

    struct DepthResources {
        VkFormat DepthFormat;
        VkImage DepthImage;
        VkImageView DepthImageView;
        VkDeviceMemory DepthImageMemory;

        void Destroy(Device& deviceRef) {
             vkDestroyImage(deviceRef.GetDevice(), DepthImage, nullptr);
             vkDestroyImageView(deviceRef.GetDevice(), DepthImageView, nullptr);
             vkFreeMemory(deviceRef.GetDevice(), DepthImageMemory, nullptr);
        }
    };

    struct GbufferData {
         VkPipeline pipeline;
         VkPipelineLayout pipelineLayout;
         VkFramebuffer framebuffer;
         VkRenderPass renderpass;
    };

     struct SSAORenderData {

          std::vector<Vertex> vertexData;
          VkBuffer vertexBuffer;
          VkPipeline ssaoPipeline;
          VkPipeline hbaoPipeline;
          VkPipeline alchchemyPipeline;
          VkPipelineLayout pipelineLayout;
          VkFramebuffer framebuffer;
          VkRenderPass renderpass;
          std::vector<VkBuffer> UniformBuffers;
          std::vector<VkDeviceMemory> UniformMemory;
          std::vector<VkBuffer> cameraUniformBuffers;
          std::vector<VkDeviceMemory> cameraUniformMemory;
          std::vector<VkDescriptorSet> descriptorSets;
     };

     struct SSAOBlur {
          VkPipeline pipeline;
          VkPipelineLayout pipelineLayout;
          VkRenderPass renderpass;
          VkFramebuffer framebuffer;
          std::vector<VkDescriptorSet> descriptorSets;
     };

     struct ImGuiData {
          VkDescriptorPool ImGuiDescriptorPool;
          VkRenderPass renderpass;
          std::vector<VkCommandBuffer> ImGuiCmds;

     };

     struct LightingPass {
          VkPipeline pipeline;
          VkPipelineLayout pipelineLayout;
          VkRenderPass renderpass;
          VkFramebuffer framebuffer;
          std::vector<VkDescriptorSet> descriptorSets;
          std::vector<VkBuffer> UniformBuffers;
          std::vector<VkDeviceMemory> UniformMemory;

          std::vector<VkDescriptorSet> ShadowdescriptorSets;
          std::vector<VkBuffer> ShadowUniformBuffers;
          std::vector<VkDeviceMemory> ShadowUniformMemory;
     };

     struct Shadows {
          VkRenderPass renderpass;
          VkFramebuffer framebuffer;
          VkPipeline pipeline;
          VkPipelineLayout pipelineLayout;
     };

    inline static SSAORenderData ssaoRenderData;
    inline static GbufferData GbufferRenderData;
    inline static ModelData SponzaData;
    inline static Quad RenderQuad;
    inline static Quad DisplayQuad;
    inline static DepthResources depthResources;
    inline static Global GlobalVK;
    inline static ImGuiData ImGuiData;
    inline static SSAOBlur SSAOBlur;
    inline static LightingPass Lighting;
    inline static Shadows Shadow;
};


struct Settings {

     enum AOState {
          SSAO,
          HBAO,
          AAO
     };
     inline static AOState AOStateController;
};


