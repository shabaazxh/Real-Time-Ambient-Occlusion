#pragma once

#include <vulkan/vulkan.h>
#define GLFW_INCLUDE_VULKAN

#include <vector>
#include <stdexcept>
#include <string>

#include "../libs/tinyobjloader/tiny_obj_loader.h"
#include "../Structs/Shaders.h"

class Model {
    public:
    Model(std::string ModelPath) {
        LoadModel(ModelPath);
    }
    void LoadModel(std::string ModelPath);

    std::vector<Vertex> GetVertexData() const { return vertices; }

    private:
    std::vector<Vertex> vertices;
};