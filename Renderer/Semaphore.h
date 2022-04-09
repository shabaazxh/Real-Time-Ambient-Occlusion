#include <vulkan/vulkan.h>
#include "../Device/Device.h"

class Semaphore {
    public:
    void SetDevice(Device& deviceRef);
    void CreateSemaphore(VkDevice device);
    void DestroySemaphore(VkDevice device);

    VkSemaphore GetSemaphore() const { return semaphore; }
    private:
    VkSemaphore semaphore;
};