#include "Semaphore.h"




void Semaphore::CreateSemaphore(VkDevice device){
    VkSemaphoreCreateInfo semaphoreInfo = {};
    semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
    
    if(vkCreateSemaphore(device, &semaphoreInfo, nullptr, &semaphore) != VK_SUCCESS){
        throw std::runtime_error("failed to create sempahore!");
    }
}

void Semaphore::DestroySemaphore(VkDevice device) {
    vkDestroySemaphore(device, semaphore, nullptr);
}