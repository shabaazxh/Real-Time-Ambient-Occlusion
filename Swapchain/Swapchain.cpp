
#include "Swapchain.h"


void SwapChain::SetWindow(GLFWwindow* window){
    m_Window = window;
}

// void SwapChain::SetDevice(VkDevice device){
//     m_Device = device;
// }

void SwapChain::SetDevice(Device& deviceRef) {
    deviceRef = deviceRef;
}

void SwapChain::DestroySwapChain() {
    for(auto framebuffer: m_swapChainFramebuffers){
        vkDestroyFramebuffer(deviceRef.GetDevice(), framebuffer, nullptr);
    }
    for(auto imageView: m_swapChainImageViews){
        vkDestroyImageView(deviceRef.GetDevice(), imageView, nullptr);
    }
    vkDestroySwapchainKHR(deviceRef.GetDevice(), m_SwapChain, nullptr);
}

void SwapChain::CreateSwapChainImageViews() {
    // Create swapchain image views for swapchain images
    m_swapChainImageViews.resize(m_swapChainImages.size());
    for(size_t i = 0; i < m_swapChainImages.size(); i++){
        VkImageViewCreateInfo info = Image::CreateImageView(m_swapChainImages[i], VK_IMAGE_VIEW_TYPE_2D, m_swapChainImageFormat, VK_IMAGE_ASPECT_COLOR_BIT, 1);

        if(vkCreateImageView(deviceRef.GetDevice(), &info, nullptr, &m_swapChainImageViews[i]) != VK_SUCCESS){
            throw std::runtime_error("failed to create swapchain image views.");
        }
    }
}

void SwapChain::CreateSwapChainFramebuffers(const VkRenderPass& renderpass) {
    
    this->renderpass = renderpass;

    m_swapChainFramebuffers.resize(m_swapChainImageViews.size());

    for(size_t i = 0; i < m_swapChainImageViews.size(); i++){
        m_swapChainFramebuffers[i] = Framebuffer::CreateFramebuffer(m_swapChainFramebuffers[i], deviceRef.GetDevice(), {m_swapChainImageViews[i]}, renderpass, m_swapChainExtent);
    }
}

void SwapChain::CreateSwapChain() {
    
    SwapChainSupportInfo swapChainSupportInfo = deviceRef.QuerySwapChainSupport(deviceRef.GetPhysicalDevice());
    
    VkSurfaceFormatKHR surfaceFormat = SelectSwapSurfaceFormat(swapChainSupportInfo.formats);
    VkPresentModeKHR presentMode = SelectSwapPresentMode(swapChainSupportInfo.presentModes);
    VkExtent2D extent = SelectSwapExtent(swapChainSupportInfo.capabilities);

    // Define amount of images in swapchain. Always use +1 to prevent wait from driver
    // to complete internal operations
    uint32_t imageCount = swapChainSupportInfo.capabilities.minImageCount + 1;

    // Ensure we do not exceed max number of images when choosing minImageCount + 1
    if(swapChainSupportInfo.capabilities.maxImageCount > 0 && imageCount > swapChainSupportInfo.capabilities.maxImageCount){
        imageCount = swapChainSupportInfo.capabilities.maxImageCount;
    }

    // Define struct to create swap chain
    VkSwapchainCreateInfoKHR createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
    createInfo.surface = m_Surface;
    createInfo.imageFormat = surfaceFormat.format;
    createInfo.imageColorSpace = surfaceFormat.colorSpace;
    createInfo.imageExtent = extent;
    createInfo.imageArrayLayers = 1;
    createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

    QueueFamilyIndices indices = deviceRef.FindQueueFamilies(deviceRef.GetPhysicalDevice(), m_Surface);
    uint32_t queueFamilyIndices[] = {indices.graphicsFamily.value(), indices.presentFamily.value()};

    if(indices.graphicsFamily != indices.presentFamily){
        createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
        createInfo.queueFamilyIndexCount = 2;
        createInfo.pQueueFamilyIndices = queueFamilyIndices;
    } else {
        createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
    }

    createInfo.preTransform = swapChainSupportInfo.capabilities.currentTransform;
    createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
    createInfo.presentMode = presentMode;
    createInfo.clipped = VK_TRUE;
    createInfo.oldSwapchain = VK_NULL_HANDLE;
    createInfo.minImageCount = imageCount;
    

    if(vkCreateSwapchainKHR(deviceRef.GetDevice(), &createInfo, nullptr, &m_SwapChain) != VK_SUCCESS) {
        throw std::runtime_error("Failed to create a swap chain");
    }

    // Retrieve swapchain images
    vkGetSwapchainImagesKHR(deviceRef.GetDevice(), m_SwapChain, &imageCount, nullptr);
    m_swapChainImages.resize(imageCount);
    vkGetSwapchainImagesKHR(deviceRef.GetDevice(), m_SwapChain, &imageCount, m_swapChainImages.data());

    m_swapChainImageFormat = surfaceFormat.format;
    m_swapChainExtent = extent;

    CreateSwapChainImageViews();
}

VkSurfaceFormatKHR SwapChain::SelectSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats) {

    // We want to look through the available formats and find
    // VK_FORMAT_B8G8R8A8_SRGB and check SRGB colour suppport using VK_COLOR_SPACE_SRGB_NONLINEAR

    for(const auto& availableFormat: availableFormats){
        if(availableFormat.format == VK_FORMAT_B8G8R8A8_SRGB && availableFormat.colorSpace ==
        VK_COLOR_SPACE_SRGB_NONLINEAR_KHR){
            return availableFormat;
        }
    }

    //if we do not find the specified settings we hopedd for, we can just take the first
    // available format
    return availableFormats[0];
}


VkPresentModeKHR SwapChain::SelectSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes){

    //Find MAILBOX present mode since it's prefered

    for(const auto& presentMode : availablePresentModes){
        if(presentMode == VK_PRESENT_MODE_MAILBOX_KHR){
            return presentMode;
        }
    }

    return VK_PRESENT_MODE_FIFO_KHR;
}

// SwapExtent refers to the resolution of the swap chain images
// Vulkan works with pixels so the swapchain extent must be specified in pixels
// glfwGetFramebufferSize is usedd to obtain resolution of window in pixels
VkExtent2D SwapChain::SelectSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities) {

    if(capabilities.currentExtent.width != std::numeric_limits<uint32_t>::max()) {
        return capabilities.currentExtent;
    } else {
        int width, height;
        glfwGetFramebufferSize(m_Window, &width, &height);

        VkExtent2D Extent = {
            static_cast<uint32_t>(width),
            static_cast<uint32_t>(height)
        };

        Extent.width = std::clamp(Extent.width, capabilities.minImageExtent.width, capabilities.maxImageExtent.width);
        Extent.height = std::clamp(Extent.height, capabilities.minImageExtent.height, capabilities.maxImageExtent.height);

        return Extent;
    }

}
