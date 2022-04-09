#pragma once

#include <vulkan/vulkan.h>
#include "../Device/Device.h"
#include "../Resources/Image.h"
#include "../libs/glm/glm.hpp"
#include "../libs/glm/gtc/matrix_transform.hpp"
#include "../libs/glm/gtx/hash.hpp"

#include <array>
#include <vector>
#include <iostream>


struct Vertex {
    glm::vec3 pos;
    glm::vec3 color;
    glm::vec2 texCoord;
    glm::vec3 normal;


    static VkVertexInputBindingDescription GetBindingDescription() {
        
        VkVertexInputBindingDescription bindingDescription{};
        bindingDescription.binding = 0;
        bindingDescription.stride = sizeof(Vertex);
        bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

        return bindingDescription;
    }

    static std::array<VkVertexInputAttributeDescription, 4> GetAttributeDescriptions() {
        std::array<VkVertexInputAttributeDescription, 4> attributeDescriptions{};
        
        attributeDescriptions[0].binding = 0;
        attributeDescriptions[0].location = 0;
        attributeDescriptions[0].format = VK_FORMAT_R32G32B32_SFLOAT;
        attributeDescriptions[0].offset = offsetof(Vertex, pos);

        attributeDescriptions[1].binding = 0;
        attributeDescriptions[1].location = 1;
        attributeDescriptions[1].format = VK_FORMAT_R32G32B32_SFLOAT;
        attributeDescriptions[1].offset = offsetof(Vertex, color);

        attributeDescriptions[2].binding = 0;
		attributeDescriptions[2].location = 2;
		attributeDescriptions[2].format = VK_FORMAT_R32G32_SFLOAT;
		attributeDescriptions[2].offset = offsetof(Vertex, texCoord);

        attributeDescriptions[3].binding = 0;
		attributeDescriptions[3].location = 3;
		attributeDescriptions[3].format = VK_FORMAT_R32G32B32_SFLOAT;
		attributeDescriptions[3].offset = offsetof(Vertex, normal);

        return attributeDescriptions;
    }
};

struct Primitives {
    std::vector<Vertex> Triangle = {
    {{-1.0f, -1.0f, 0.0f}, {1.0f, 0.0f, 0.0f}, {0.0f, 0.0f}},
    {{1.0f, -1.0f, 0.0f}, {0.0f, 1.0f, 0.0f},  {1.0f, 0.0f}},
    {{1.0f, 1.0f, 0.0f}, {0.0f, 0.0f, 1.0f},   {1.0f, 1.0f}},
    {{-1.0f, 1.0f, 0.0f}, {1.0f, 1.0f, 0.0f},  {0.0f, 1.0f}},
    {{-1.0f, -1.0f, 0.0f}, {1.0f, 0.0f, 0.0f}, {0.0f, 0.0f}},
    {{1.0f, 1.0f, 0.0f}, {0.0f, 0.0f, 1.0f},   {1.0f, 1.0f}},
    };
};

struct CameraProjection {
    alignas(16) glm::mat4 model;
    alignas(16) glm::mat4 view;
    alignas(16) glm::mat4 proj;
};

struct Light {
    alignas(16) glm::vec4 LightPosition;
    alignas(16) glm::vec4 ObjectColor;
    alignas(16) glm::vec4 LightColor;
    alignas(16) glm::vec4 LightViewPosition;
    alignas(16) glm::mat4 LightSpaceMatrix;
};

struct SSAOubo {
    alignas(16) glm::vec4 samples[64];
    alignas(4) int sample_amount;
    alignas(4) float radius;

    alignas(4) int hbaoSampleDirection;
    alignas(4) float hbaoSteps;
    alignas(4) int hbaoNumberOfSteps;
    alignas(4) float hbaoAmbientLightLevel;

    alignas(4) int alchemySampleTurns;
    alignas(4) float alchemySigma;
    alignas(4) float alchemyKappa;
};

struct SSAOuboController {
    inline static int samples;
    inline static float radius;

    inline static int hbaoSampleDirection;
    inline static float hbaoSteps;
    inline static int hbaoNumberOfSteps;
    inline static float hbaoAmbientLightLevel;

    inline static int alchemySampleTurns;
    inline static float alchemySigma;
    inline static float alchemyKappa;
};

struct RenderPresentSettings {
    alignas(1) bool enableBlur;
    alignas(1) bool enableLight;
};

struct RenderPresentSettingsController {
    inline static bool enableBlur = true;
    inline static bool enableLight = false;
};

inline float Lerp(float a, float b, float f) {
    return a + f * (b - a);
}

// Descriptors 
inline VkDescriptorSetLayoutBinding CreateDescriptorBinding(int binding, VkDescriptorType type, VkShaderStageFlags stage) {

    VkDescriptorSetLayoutBinding bindingLayout = {};
    bindingLayout.binding = binding;
    bindingLayout.descriptorType = type;
    bindingLayout.descriptorCount = 1;
    bindingLayout.stageFlags = stage;

    return bindingLayout;
}

// Buffer descriptor
inline void UpdateDescriptorSet(int binding, Device& deviceRef, std::vector<VkBuffer>& buffers, std::vector<VkDescriptorSet> descriptorSets, size_t range, VkDescriptorType type){

    for(size_t i = 0; i < 3; i++) {
        VkDescriptorBufferInfo bufferinfo = {};
        bufferinfo.buffer = buffers[i];
        bufferinfo.offset = 0;
        bufferinfo.range = range;

        VkWriteDescriptorSet descriptorWrite{};
        descriptorWrite.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
        descriptorWrite.dstSet = descriptorSets[i];
        descriptorWrite.dstBinding = binding;
        descriptorWrite.dstArrayElement = 0;
        descriptorWrite.descriptorType = type;
        descriptorWrite.descriptorCount = 1;
        descriptorWrite.pBufferInfo = &bufferinfo;

        std::vector<VkWriteDescriptorSet> writeDescriptorSets =
        {
                descriptorWrite
        };

        vkUpdateDescriptorSets(deviceRef.GetDevice(), static_cast<uint32_t>(writeDescriptorSets.size()), writeDescriptorSets.data(), 0, nullptr);
    }
}

// Image descriptor
inline void UpdateDescriptorSet(int binding, Device& deviceRef, std::vector<VkDescriptorSet> descriptorSets, VkDescriptorType type, VkImageView imageView, VkSampler sampler, VkImageLayout layout){

    for(size_t i = 0; i < 3; i++) {

        VkDescriptorImageInfo imageInfo = {};
        imageInfo.imageLayout = layout;
        imageInfo.imageView = imageView;
        imageInfo.sampler = sampler;

        VkWriteDescriptorSet descriptorWrite{};
        descriptorWrite.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
        descriptorWrite.dstSet = descriptorSets[i];
        descriptorWrite.dstBinding = binding;
        descriptorWrite.dstArrayElement = 0;
        descriptorWrite.descriptorType = type;
        descriptorWrite.descriptorCount = 1;
        descriptorWrite.pImageInfo = &imageInfo;

        std::vector<VkWriteDescriptorSet> writeDescriptorSets =
        {
                descriptorWrite
        };

        vkUpdateDescriptorSets(deviceRef.GetDevice(), static_cast<uint32_t>(writeDescriptorSets.size()), writeDescriptorSets.data(), 0, nullptr);
    }

}

struct DesciptorSetAllocate {

    DesciptorSetAllocate(Device& deviceRef, VkDescriptorPool descriptorPool, VkDescriptorSetLayout layout, std::vector<VkDescriptorSet>& descriptorSets, int count) {
        
        std::vector<VkDescriptorSetLayout> layouts(count, layout);

        VkDescriptorSetAllocateInfo allocInfo = {};
        allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
        allocInfo.descriptorPool = descriptorPool;
        allocInfo.descriptorSetCount = count;
        allocInfo.pSetLayouts = layouts.data();

        descriptorSets.resize(3);
        if(vkAllocateDescriptorSets(deviceRef.GetDevice(), &allocInfo, descriptorSets.data()) != VK_SUCCESS){
            throw std::runtime_error("failed to create/allocate descriptor sets");
        }

    }
};




