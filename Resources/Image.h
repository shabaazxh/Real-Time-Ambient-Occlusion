#pragma once

#include <vulkan/vulkan.h>
#include "../Device/Device.h"
#include "../Commands/Commandbuffer.h"

#include "Memory.h"
#include <stdexcept>
#include <cmath>
#include <iostream>

class Image {
    public:
        Image(Device& deviceRef) : deviceRef{deviceRef} {}
        void DestroyImageResources();

        static VkImageViewCreateInfo CreateImageView(VkImage image, VkImageViewType viewType, VkFormat imageFormat, VkImageAspectFlags aspectFlags);
        
        void CreateSampler();
        void SetImageView(VkImageView imageView);
        void CreateImage(uint32_t width, uint32_t height, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage, VkMemoryPropertyFlags properties);
        void BuildImageView(VkImage image, VkImageViewType viewType, VkFormat imageFormat, VkImageAspectFlags aspectFlags);
        void CreateImageResource(uint32_t width, uint32_t height, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage, VkMemoryPropertyFlags properties, VkImageAspectFlags aspectFlags);
        void TransitionImageLayout(VkCommandPool commandPool, VkImage image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout);
        void CopyBufferToImage(VkCommandPool commandPool, VkBuffer buffer, VkImage image, uint32_t width, uint32_t height);

        VkImage GetImage() const { return m_Image; }
        VkImageView GetImageView() const { return m_ImageView; }
        VkDeviceMemory GetImageMemory() const { return m_ImageMemory; }
        VkSampler GetSampler() const { return m_Sampler; }

    private:

    Device& deviceRef;
    VkImageView m_ImageView = VK_NULL_HANDLE;
    VkImage m_Image = VK_NULL_HANDLE;
    VkDeviceMemory m_ImageMemory;
    VkSampler m_Sampler = VK_NULL_HANDLE;
};

inline VkFormat FindSupportedDepthFormat(Device& deviceRef, const std::vector<VkFormat>& candidates, VkImageTiling tiling, VkFormatFeatureFlags features)
{
    for(VkFormat format : candidates){
        VkFormatProperties props;
        vkGetPhysicalDeviceFormatProperties(deviceRef.GetPhysicalDevice(), format, &props);

        if(tiling == VK_IMAGE_TILING_LINEAR && (props.linearTilingFeatures & features) == features){
            return format;
        } else if(tiling == VK_IMAGE_TILING_OPTIMAL && (props.optimalTilingFeatures & features) == features){
            return format;
        }
    }

    throw std::runtime_error("failed to find supported format");
}

inline VkFormat FindDepthFormat(Device& deviceRef) {
    return FindSupportedDepthFormat(deviceRef, {VK_FORMAT_D32_SFLOAT, VK_FORMAT_D32_SFLOAT_S8_UINT, VK_FORMAT_D24_UNORM_S8_UINT},
    VK_IMAGE_TILING_OPTIMAL,
    VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT);
}






