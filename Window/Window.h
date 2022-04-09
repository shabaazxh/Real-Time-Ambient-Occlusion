
#pragma once

#include <vulkan/vulkan.h>
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <vector>
#include <stdexcept>

class Window {
    public:
        std::vector<const char*> GetRequiredExtensions();
        void CreateWindow(int width, int height, const char* title);
        void CreateSurface(VkInstance& instance);
        void LockWindowCamera(bool enable);
        void DestroyWindow();
        void DestroySurface(VkInstance instance);
        VkSurfaceKHR GetSurface() const { return m_Surface; }
        GLFWwindow* GetWindow() const { return m_Window; }

    private:
    GLFWwindow* m_Window;
    VkSurfaceKHR m_Surface;

};