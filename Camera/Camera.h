#pragma once

#include <vulkan/vulkan.h>
#include <vector>
#include <chrono>
#include <array>
#include <random>
#include <memory>

#include "../libs/glm/glm.hpp"
#include "../libs/glm/gtc/matrix_transform.hpp"
#include "../libs/glm/gtx/hash.hpp"
#include "../Commands/Commandbuffer.h"
#include "../Swapchain/Swapchain.h"
#include "../Window/Window.h"
#include "../Resources/Buffer.h"
#include "../Renderer/RenderData.h"
#include "../Structs/Shaders.h"



class Camera {
    public:
    struct MouseControlSettings {
        float yaw = -90.0f;
        float pitch = 0.0f;
        float lastX = 1920 / 2.0f;
        float lastY = 1080 / 2.0f;
    };

    Camera()  {
        SetCameraPos(glm::vec3(0.0f, 0.0f, 0.0f));
        m_CameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
        m_CameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
    }
    Camera operator=(const Camera& camera)
    {
        this->MouseSettings = camera.MouseSettings;
        this->m_CameraPos = camera.m_CameraPos;
        this->m_CameraFront = camera.m_CameraFront;
        this->m_CameraUp = camera.m_CameraUp;
    }


    void MouseControl(GLFWwindow* window, double xOffset, double yOffset);
    void SetCameraPos(glm::vec3 newPosition);
    void SetCameraLock(bool lock);


    glm::vec3 GetCameraPos()  { return m_CameraPos; }
    glm::vec3 GetCameraFront() { return m_CameraFront; }
    glm::vec3 GetCameraUp() { return m_CameraUp; }
    bool GetCameraLock()  {return  m_LockCamera; }
    
      
    private:
    MouseControlSettings MouseSettings;
    glm::vec3 m_CameraPos;
    glm::vec3 m_CameraFront;
    glm::vec3 m_CameraUp;
    bool m_LockCamera = false;
    bool m_FirstMouse;
    
};

inline static Camera* mouse_input;
static void MouseCallback(GLFWwindow* window, double x, double y) {
    mouse_input->MouseControl(window, x, y);
}

