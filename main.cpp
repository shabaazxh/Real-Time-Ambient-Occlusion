#include "Application/Application.h"


int main()
{
    Application app(true);

    try{
        app.run();
    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        return 1;
    }

    return 0;
}