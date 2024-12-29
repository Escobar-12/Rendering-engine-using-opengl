#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm.hpp>

class Camera {
private:
    GLFWwindow* window;
    float speed = 3.0f;
    float Shiftspeed = 2.5f;
    
    
public:
    Camera();
    ~Camera();
    bool firstframe = true;
    void Handle_Input(glm::vec3& cameraPos, glm::vec3& cameraFront, glm::vec3& cameraUp);
    void UpdateMatrices(unsigned int viewMatLocation, unsigned int projectionLocation, glm::vec3 cameraPos, glm::vec3 cameraFront, glm::vec3 cameraUp);
    void MouseCamMovement();
    void mouse_callback(GLFWwindow* window, double MouseX, double MouseY);
    glm::vec3 CameraForward();
    GLFWwindow* CamReturnContextWindow();
    float near = 0.1f;
    float far = 100.0f;
};

void ScrollCallBack(GLFWwindow* window, double ScrollX, double ScrollY);
void MouseButtonCallBack(GLFWwindow* window, int button, int action, int mods);
void DrawCircle(int steps, int radius, glm::vec2 Pos);
void WindowSizeCallBack(GLFWwindow* window, int width, int height);

