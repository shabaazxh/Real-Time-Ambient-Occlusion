#include "Window.h"


std::vector<const char*> Window::GetRequiredExtensions() {
    uint32_t glfwExtensionCount = 0;
    const char** glfwExtenions;
    glfwExtenions= glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

    std::vector<const char*> extensions(glfwExtenions, glfwExtenions + glfwExtensionCount);

    return extensions;

}


void Window::CreateWindow(int width, int height, const char* title) {

    glfwInit();
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
    m_Window = glfwCreateWindow(width, height, title, nullptr, nullptr);
    glfwSetInputMode(m_Window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
}

void Window::LockWindowCamera(bool enable) {

    if(enable == true)
    {
        glfwSetInputMode(m_Window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    } else {
        glfwSetInputMode(m_Window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    }
    
}

void Window::CreateSurface(VkInstance& instance) {
    if(glfwCreateWindowSurface(instance, m_Window, nullptr, &m_Surface) != VK_SUCCESS) {
        throw std::runtime_error("Failed to create window surface.");
    }
}

void Window::DestroyWindow() {
    glfwDestroyWindow(m_Window);
}

void Window::DestroySurface(VkInstance instance) {
    vkDestroySurfaceKHR(instance, m_Surface, nullptr);
}