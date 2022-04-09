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

         void Destroy(Device& deviceRef){

               //for(size_t i = 0; i < 2; i++){
               //     vkDestroyBuffer(deviceRef.GetDevice(), UniformBuffers[i], nullptr);
               //     vkFreeMemory(deviceRef.GetDevice(), UniformMemory[i], nullptr);
               //}

               //if(vertexBuffer != VK_NULL_HANDLE){
               //     vkDestroyBuffer(deviceRef.GetDevice(), vertexBuffer, nullptr);
               //}

               vkDestroyFramebuffer(deviceRef.GetDevice(), framebuffer, nullptr);
               vkDestroyRenderPass(deviceRef.GetDevice(), renderpass, nullptr);
               vkDestroyPipeline(deviceRef.GetDevice(), pipeline, nullptr);
               vkDestroyPipelineLayout(deviceRef.GetDevice(), pipelineLayout, nullptr);
          }
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

         void Destroy(Device& deviceRef){

               //for(size_t i = 0; i < 2; i++){
               //     vkDestroyBuffer(deviceRef.GetDevice(), UniformBuffers[i], nullptr);
               //     vkFreeMemory(deviceRef.GetDevice(), UniformMemory[i], nullptr);
               //}

               //if(vertexBuffer != VK_NULL_HANDLE){
               //     vkDestroyBuffer(deviceRef.GetDevice(), vertexBuffer, nullptr);
               //}

               vkDestroyFramebuffer(deviceRef.GetDevice(), framebuffer, nullptr);
               vkDestroyRenderPass(deviceRef.GetDevice(), renderpass, nullptr);
               vkDestroyPipeline(deviceRef.GetDevice(), pipeline, nullptr);
               vkDestroyPipelineLayout(deviceRef.GetDevice(), pipelineLayout, nullptr);
         }
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

         void Destroy(Device& deviceRef){
               vkDestroyFramebuffer(deviceRef.GetDevice(), framebuffer, nullptr);
               vkDestroyRenderPass(deviceRef.GetDevice(), renderpass, nullptr);
               vkDestroyPipeline(deviceRef.GetDevice(), pipeline, nullptr);
               vkDestroyPipelineLayout(deviceRef.GetDevice(), pipelineLayout, nullptr);
         }
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

          void Destroy(Device& deviceRef){

               for(size_t i = 0; i < 2; i++){
                    //vkDestroyBuffer(deviceRef.GetDevice(), UniformBuffers[i], nullptr);
                    // vkDestroyBuffer(deviceRef.GetDevice(), cameraUniformBuffers[i], nullptr);
                    // vkFreeMemory(deviceRef.GetDevice(), cameraUniformMemory[i], nullptr);
                    //vkFreeMemory(deviceRef.GetDevice(), UniformMemory[i], nullptr);
               }

               //if(vertexBuffer != VK_NULL_HANDLE){
               //     vkDestroyBuffer(deviceRef.GetDevice(), vertexBuffer, nullptr);
               //}

               vkDestroyFramebuffer(deviceRef.GetDevice(), framebuffer, nullptr);
               vkDestroyRenderPass(deviceRef.GetDevice(), renderpass, nullptr);
               vkDestroyPipeline(deviceRef.GetDevice(), ssaoPipeline, nullptr);
               vkDestroyPipeline(deviceRef.GetDevice(), hbaoPipeline, nullptr);
               vkDestroyPipeline(deviceRef.GetDevice(), alchchemyPipeline, nullptr);
               vkDestroyPipelineLayout(deviceRef.GetDevice(), pipelineLayout, nullptr);
         }
     };

     struct SSAOBlur {
          VkPipeline pipeline;
          VkPipelineLayout pipelineLayout;
          VkRenderPass renderpass;
          VkFramebuffer framebuffer;
          std::vector<VkDescriptorSet> descriptorSets;

          void Destroy(Device& deviceRef){
               vkDestroyFramebuffer(deviceRef.GetDevice(), framebuffer, nullptr);
               vkDestroyRenderPass(deviceRef.GetDevice(), renderpass, nullptr);
               vkDestroyPipeline(deviceRef.GetDevice(), pipeline, nullptr);
               vkDestroyPipelineLayout(deviceRef.GetDevice(), pipelineLayout, nullptr);
         }
     };

     struct ImGuiData {
          VkDescriptorPool ImGuiDescriptorPool;
          VkRenderPass renderpass;
          std::vector<VkCommandBuffer> ImGuiCmds;

          void Destroy(Device& deviceRef)
          {
               vkDestroyRenderPass(deviceRef.GetDevice(), renderpass, nullptr);
               vkDestroyDescriptorPool(deviceRef.GetDevice(), ImGuiDescriptorPool, nullptr);
          }
     };

     struct LightingPass {
          VkPipeline pipeline;
          VkPipelineLayout pipelineLayout;
          VkRenderPass renderpass;
          VkFramebuffer framebuffer;
          std::vector<VkDescriptorSet> descriptorSets;
          std::vector<VkBuffer> UniformBuffers;
          std::vector<VkDeviceMemory> UniformMemory;

          void Destroy(Device& deviceRef){

               //for(size_t i = 0; i < 2; i++){
               //     vkDestroyBuffer(deviceRef.GetDevice(), UniformBuffers[i], nullptr);
               //     vkFreeMemory(deviceRef.GetDevice(), UniformMemory[i], nullptr);
               //}

               vkDestroyFramebuffer(deviceRef.GetDevice(), framebuffer, nullptr);
               vkDestroyRenderPass(deviceRef.GetDevice(), renderpass, nullptr);
               vkDestroyPipeline(deviceRef.GetDevice(), pipeline, nullptr);
               vkDestroyPipelineLayout(deviceRef.GetDevice(), pipelineLayout, nullptr);
         }
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
};


struct Settings {

     enum AOState {
          SSAO,
          HBAO,
          AAO
     };
     inline static AOState AOStateController;
};


