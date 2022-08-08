#include "Device.h"

void Device::SetSurface(VkSurfaceKHR surface){
    m_Surface = surface;
}

void Device::CreateDevice(){
    QueueFamilyIndices indices = FindQueueFamilies(m_physicalDevice, m_Surface);

    // Since we can have multiple queues, instead of creating individual struct infos, just store
    // vector of theh struct to set properties using for loop
    std::vector<VkDeviceQueueCreateInfo> allQueueCreateInfo;
    std::set<uint32_t> InUseQueueFamilies = {indices.graphicsFamily.value(), indices.presentFamily.value()};

    // We can assign priorities to queues to influence command buffer execution. Requires
    // Even if only queue family.
    float queuePriority = 1.0f;
    for(uint32_t queueFamily: InUseQueueFamilies){
        // Create graphics queue on this device.
        VkDeviceQueueCreateInfo queueCreateInfo{};
        queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
        queueCreateInfo.queueFamilyIndex = queueFamily;
        queueCreateInfo.queueCount = 1;
        queueCreateInfo.pQueuePriorities = &queuePriority;
        allQueueCreateInfo.push_back(queueCreateInfo);
    }

    // We can specify certain desired device features if needed.
    VkPhysicalDeviceFeatures deviceFeatures{};

    // Device creation info struct
    VkDeviceCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    createInfo.pQueueCreateInfos = allQueueCreateInfo.data(); // pass the  queue creation info
    createInfo.queueCreateInfoCount = static_cast<uint32_t>(allQueueCreateInfo.size());
    createInfo.enabledExtensionCount = static_cast<uint32_t>(DeviceExtensions.size());
    createInfo.ppEnabledExtensionNames = DeviceExtensions.data();
    createInfo.pEnabledFeatures = &deviceFeatures; //  pass the device features

    if(vkCreateDevice(m_physicalDevice, &createInfo, nullptr, &m_Device) != VK_SUCCESS){
        throw std::runtime_error("Failed to create a logical device.");
    }

    // Create a handle to the graphics queue
    vkGetDeviceQueue(m_Device, indices.graphicsFamily.value(), 0, &m_graphicsQueue);
    // Create a handle to the present queue
    vkGetDeviceQueue(m_Device, indices.presentFamily.value(), 0, &m_presentQueue);

}

void Device::SelectGPUDevice(VkInstance instance)
{
    // List graphics cards by first querying the number of them we have.
    uint32_t deviceCount = 0;
    vkEnumeratePhysicalDevices(instance, &deviceCount, nullptr); 

    // if device count is 0, terminate since we cannot use Vulkan
    if(deviceCount == 0){
        throw std::runtime_error("Failed to find any GPU that supports Vulkan.");
    }

    // If a device exists with Vulkan support, store all available devices.
    std::vector<VkPhysicalDevice> devices(deviceCount);
    vkEnumeratePhysicalDevices(instance, &deviceCount, devices.data());

    // For each found device, check if it's suitable for our operations
    for(const auto& device: devices){
        if(DeviceSuitability(device)){
            m_physicalDevice = device;
            break;
        }
    }

    if(m_physicalDevice == VK_NULL_HANDLE){
        throw std::runtime_error("Failed to find a suitable GPU");
    }


}

// Check the device for suitability. Can it perform the operations we want?
// Since we want to just check for vulkan support we return true
bool Device::DeviceSuitability(VkPhysicalDevice physicalDevice) {
   //Ensure our device can perform graphics operations
   QueueFamilyIndices indices = FindQueueFamilies(physicalDevice, m_Surface);

    // check the swapchain extension support
   bool deviceExtensionSupport = CheckDeviceExtensios(physicalDevice);

   bool SwapChainAdequate = false;
   // check if we have swapchain support
   if(deviceExtensionSupport){
       SwapChainSupportInfo swapChainSupport = QuerySwapChainSupport(physicalDevice);
       SwapChainAdequate = !swapChainSupport.formats.empty() && !swapChainSupport.presentModes.empty();
   }

    //isComplete() will check if all requires families have a value = supported
   return indices.isComplete() && deviceExtensionSupport && SwapChainAdequate;

}

bool Device::CheckDeviceExtensios(VkPhysicalDevice pDevice) {
    // Get the number of extensions
    uint32_t extensionCount;
    vkEnumerateDeviceExtensionProperties(pDevice, nullptr, &extensionCount, nullptr);

    // Store all found extensions
    std::vector<VkExtensionProperties> foundExtensions(extensionCount);
    vkEnumerateDeviceExtensionProperties(pDevice, nullptr, &extensionCount, foundExtensions.data());

    std::set<std::string> RequiredExtensions(DeviceExtensions.begin(), DeviceExtensions.end());
    // We will loop through all found extensions and if we see a required extension
    // We will remove it from the required extensions list to note it's been found
    for(const auto& extension: foundExtensions){
        RequiredExtensions.erase(extension.extensionName);
    }

    return RequiredExtensions.empty();
}

QueueFamilyIndices Device::FindQueueFamilies(VkPhysicalDevice pDevice, VkSurfaceKHR surface){
    QueueFamilyIndices indices;

    //Retrieve list of queue families 
    uint32_t queueFamilyCount = 0;
    vkGetPhysicalDeviceQueueFamilyProperties(pDevice, &queueFamilyCount, nullptr);

    //Store all found families
    std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
    vkGetPhysicalDeviceQueueFamilyProperties(pDevice, &queueFamilyCount, queueFamilies.data());

    //Stored families contain some information such as the type of family. We need to find done
    // that supports graphics operations so we look for VK_QUEUE_GRAPHICS_BIT

    int i = 0;
    for(const auto& queueFamily : queueFamilies){

        VkBool32 presentSupport = false;
        vkGetPhysicalDeviceSurfaceSupportKHR(pDevice, i, surface, &presentSupport);

        if(queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT){
            indices.graphicsFamily = i;
        }

        if(presentSupport){
            indices.presentFamily = i;
        }

        if(indices.isComplete()){
            break;
        }
        i++;
    }

    return indices;
}

SwapChainSupportInfo Device::QuerySwapChainSupport(VkPhysicalDevice pDevice) {
    SwapChainSupportInfo supportInfoDetails;

    //Query for swapchain capabilities
    vkGetPhysicalDeviceSurfaceCapabilitiesKHR(pDevice, m_Surface, &supportInfoDetails.capabilities);

    // Query for all surface formats
    uint32_t formatCount;
    vkGetPhysicalDeviceSurfaceFormatsKHR(pDevice, m_Surface, &formatCount, nullptr);

    if(formatCount != 0){
        supportInfoDetails.formats.resize(formatCount);
        vkGetPhysicalDeviceSurfaceFormatsKHR(pDevice, m_Surface, &formatCount, supportInfoDetails.formats.data());

    }

    // Query for all present modes
    uint32_t presentModeCount;
    vkGetPhysicalDeviceSurfacePresentModesKHR(pDevice, m_Surface, &presentModeCount, nullptr);

    if(presentModeCount != 0){
        supportInfoDetails.presentModes.resize(presentModeCount);
        vkGetPhysicalDeviceSurfacePresentModesKHR(pDevice, m_Surface, &presentModeCount, supportInfoDetails.presentModes.data());

    }

    return supportInfoDetails;
}

void Device::DestroyDevice() {
    if(m_Device != VK_NULL_HANDLE)
    {
        vkDestroyDevice(m_Device, nullptr);
    }
}