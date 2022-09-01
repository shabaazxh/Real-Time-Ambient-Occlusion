#include <vulkan/vulkan.h>


const int swapchain_images = 3;

struct context {
     VkInstance instance;
};



namespace vk {

    void create_vulkan_instance();
}; //namespace vk