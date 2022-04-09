#include "Camera.h"


void Camera::MouseControl(GLFWwindow* window, double xOffset, double yOffset) {
    
    if(m_LockCamera == false) {

        if(m_FirstMouse) {
            MouseSettings.lastX = xOffset;
            MouseSettings.lastY = yOffset;
            m_FirstMouse = false;
        }

        float xoffset = xOffset - MouseSettings.lastX;
        float yoffset = MouseSettings.lastY - yOffset;
        MouseSettings.lastX = xOffset;
        MouseSettings.lastY = yOffset;

        float sensitivity = 0.1f;
        xoffset *= sensitivity;
        yoffset *= sensitivity;

        MouseSettings.yaw += xoffset;
        MouseSettings.pitch += yoffset;

        if(MouseSettings.pitch > 89.0f) {
            MouseSettings.pitch = 89.0f;
        }
        if(MouseSettings.pitch < -89.0f){
            MouseSettings.pitch = -89.0f;
        }

        glm::vec3 direction;
        direction.x = cos(glm::radians(MouseSettings.yaw)) * cos(glm::radians(MouseSettings.pitch));
        direction.y = sin(glm::radians(MouseSettings.pitch));
        direction.z = sin(glm::radians(MouseSettings.yaw)) * cos(glm::radians(MouseSettings.pitch));
        m_CameraFront = glm::normalize(direction);
    }
}

void Camera::SetCameraPos(glm::vec3 newPosition)
{
    m_CameraPos = newPosition;
}

void Camera::SetCameraLock(bool lock){
    m_LockCamera = lock;
}
