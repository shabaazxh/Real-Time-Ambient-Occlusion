#include "Model.h"


void Model::LoadModel(std::string ModelPath){

    tinyobj::attrib_t attrib;
    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> materials;
    std::string warn, err;

    if(!tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, ModelPath.c_str())) {
        throw std::runtime_error(warn + err);
    }

    for(const auto& shape: shapes){
        for(const auto& index : shape.mesh.indices){

            Vertex vertex{};

            vertex.pos = {
                attrib.vertices[3 * index.vertex_index + 0],
                attrib.vertices[3 * index.vertex_index + 1],
                attrib.vertices[3 * index.vertex_index + 2]
            };

            vertex.normal = {
                attrib.normals[3 * index.normal_index + 0],
                attrib.normals[3 * index.normal_index + 1],
                attrib.normals[3 * index.normal_index + 2]
			};

            vertex.color = {1.0f, 1.0f, 1.0f};

            vertices.push_back(vertex);
        }
    }

}