#pragma once

#include <vulkan/vulkan.h>
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <vector>
#include <stdexcept>
class Window {
    public:
        std::vector<const char*> GetRequiredExtensions();
        void CreateWindow(const char* title);
        void CreateSurface(VkInstance& instance);
        void LockWindowCamera(bool enable);
        void DestroyWindow();
        void DestroySurface(VkInstance instance);

        VkSurfaceKHR GetSurface() const { return m_Surface; }
        GLFWwindow* GetWindow() const { return m_Window; }
        uint32_t GetWindowWidth() const { return m_width; }
        uint32_t GetWindowHeight() const { return m_height; }
        bool GetFramebufferResized() const { return framebufferResized; }
        
        void SetFramebufferResized(bool framebuffer_resize);
        static void framebufferResizeCallback(GLFWwindow* window, int with, int height);
        
    private:
        GLFWwindow* m_Window;
        VkSurfaceKHR m_Surface;
        bool framebufferResized;
        uint32_t m_width = 1920;
        uint32_t m_height = 1080;

};