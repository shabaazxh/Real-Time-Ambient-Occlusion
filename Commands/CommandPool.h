#include <vulkan/vulkan.h>
#include "../Device/Device.h"
#include "../Window/Window.h"

// CommandPool needs to be created to allocate memory for commandBuffers

class CommandPool {
    public:
    CommandPool(Device& deviceRef, Window& windowRef) : deviceRef{deviceRef}, windowRef{windowRef} {}

    void CreateCommandPool();
    void DestroyCommandPool();
    VkCommandPool GetCommandPool() const { return m_CommandPool; }

    private:
    Device& deviceRef;
    Window& windowRef;
    VkCommandPool m_CommandPool;
};