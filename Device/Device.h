#pragma once
#include <vulkan/vulkan.h>
#include <vector>
#include <stdexcept>
#include <optional>
#include <set>

struct QueueFamilyIndices {
    std::optional<uint32_t> graphicsFamily;
    std::optional<uint32_t> presentFamily;
    
    bool isComplete(){
        return graphicsFamily.has_value() && presentFamily.has_value();
    }
};

struct SwapChainSupportInfo {
    VkSurfaceCapabilitiesKHR capabilities; // capabilities such as min/max number of images in swapchain
    std::vector<VkSurfaceFormatKHR> formats; // surface formats
    std::vector<VkPresentModeKHR> presentModes; // available present modes
};

class Device {

public:

    Device& operator=(const Device& other) {
        this->m_Device = other.GetDevice();
        this->m_physicalDevice = other.GetPhysicalDevice();
        this->m_graphicsQueue = other.GetGraphicsQueue();
        this->m_presentQueue = other.GetPresentQueue();

        return *this;
    }

    void SelectGPUDevice(VkInstance instance);
    void CreateDevice();
    void DestroyDevice();
    bool DeviceSuitability(VkPhysicalDevice physicalDevice);
    bool CheckDeviceExtensios(VkPhysicalDevice pDevice);
    void SetSurface(VkSurfaceKHR surface);

    VkPhysicalDevice GetPhysicalDevice() const { return m_physicalDevice; }
    VkDevice GetDevice() const { return m_Device; }
    QueueFamilyIndices FindQueueFamilies(VkPhysicalDevice pDevice, VkSurfaceKHR surface);
    SwapChainSupportInfo QuerySwapChainSupport(VkPhysicalDevice pDevice);
    VkQueue GetGraphicsQueue() const { return m_graphicsQueue; }
    VkQueue GetPresentQueue() const { return m_presentQueue; }

private:
    VkPhysicalDevice m_physicalDevice = VK_NULL_HANDLE;
    VkDevice m_Device = VK_NULL_HANDLE;
    VkQueue m_graphicsQueue;
    VkQueue m_presentQueue;
    VkSurfaceKHR m_Surface;

    const std::vector<const char*> DeviceExtensions = {
        VK_KHR_SWAPCHAIN_EXTENSION_NAME
    };
};