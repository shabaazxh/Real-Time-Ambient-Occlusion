#pragma once

#include <vulkan/vulkan.h>
#include <vector>
#include <stdexcept>
#include <optional>
#include <set>
#include <limits>
#include <algorithm>
#include <cstdint>
#include <GLFW/glfw3.h>
#include "../Device/Device.h"
#include "../Resources/Image.h"
#include "../Framebuffer/Framebuffer.h"

class SwapChain {

    public:
        SwapChain() = default;
        SwapChain(Device& deviceRef, VkSurfaceKHR surface) : deviceRef{deviceRef}, m_Surface{surface} {
            CreateSwapChain();
        }
        SwapChain& operator=(const SwapChain& other){

            this->deviceRef = other.deviceRef;
            this->m_SwapChain = other.m_SwapChain;
            this->m_swapChainImageViews = other.m_swapChainImageViews;
            this->m_swapChainImageFormat = other.m_swapChainImageFormat;
            this->m_swapChainExtent = other.m_swapChainExtent;
            this->renderpass = other.renderpass;
            this->m_Device = other.m_Device;
            this->m_Window = other.m_Window;
            this->m_Surface = other.m_Surface;

            return *this;
        }

        void CreateSwapChain();
        void DestroySwapChain();
        void SetWindow(GLFWwindow* window);
        void SetDevice(Device& deviceRef);
        void CreateSwapChainImageViews();
        void CreateSwapChainFramebuffers(const VkRenderPass& renderpass);

        // Swapchain settings
        VkSurfaceFormatKHR SelectSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);
        VkPresentModeKHR SelectSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes);
        VkExtent2D SelectSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities);
        VkSwapchainKHR GetSwapChain() const { return m_SwapChain; }

        std::vector<VkImageView> GetSwapChainImageViews() const { return m_swapChainImageViews; }
        std::vector<VkImage> GetSwapChainImages() const { return m_swapChainImages; }
        std::vector<VkFramebuffer> GetSwapChainFramebuffers() const { return m_swapChainFramebuffers; }
        VkFormat GetSwapChainFormat() const { return m_swapChainImageFormat; }
        VkExtent2D GetSwapChainExtent() const { return m_swapChainExtent; }
        VkRenderPass GetSwapChainRenderpass() const { return renderpass; }

    private:

        Device &deviceRef;
        VkDevice m_Device;
        VkSwapchainKHR m_SwapChain;
        GLFWwindow* m_Window;
        VkSurfaceKHR m_Surface;
        VkRenderPass renderpass;

        std::vector<VkImageView> m_swapChainImageViews;
        std::vector<VkImage> m_swapChainImages;
        std::vector<VkFramebuffer> m_swapChainFramebuffers;
        VkFormat m_swapChainImageFormat;
        VkExtent2D m_swapChainExtent;

};