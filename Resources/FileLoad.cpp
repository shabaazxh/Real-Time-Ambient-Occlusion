#include "FileLoad.h"


std::vector<char> FileLoader::LoadFile(const std::string& filepath)
{
    // Open the file with ate and binary flags in ordder to use tellg to obtain file size
    std::ifstream file(filepath, std::ios::ate | std::ios::binary);

    if(!file.is_open()){
        throw std::runtime_error("failed to open shader file.");
    }

    size_t fileSize = (size_t) file.tellg();
    std::vector<char> buffer(fileSize);

    file.seekg(0); //set first character to beginining of the file stream
    file.read(buffer.data(), fileSize); // read contents into buffer vector

    file.close();

    return buffer;
}