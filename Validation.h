#include <vulkan/vulkan.h>
#include <vector>

struct Validation {

    bool enableValidationLayers = false;

    const std::vector<const char*> validationLayers = {
        "VK_LAYER_KHRONOS_validation"
    };

    bool ValidationLayersSupport(){
        // Find how many there are
        uint32_t layerCount;
        vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

        // Obtain further details on these properties
        std::vector<VkLayerProperties> availableLayers(layerCount);
        vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

        // Check if all layers specified inside validation layers exists inside availableLayers
        // to confirm if the layers we want our available to us.
        for(const char* layerName: validationLayers){
            bool LayerFound = false;

            for(const auto& layerProperties : availableLayers){
                if(strcmp(layerName, layerProperties.layerName) == 0) {
                    // Matching
                    LayerFound = true;
                    break;
                }
            }
            if(!LayerFound){
                return false;
            }
        }
        return true;
    }

};