#include <fstream>
#include <vector>

class FileLoader {
    public:
    static std::vector<char> LoadFile(const std::string& filepath);
};