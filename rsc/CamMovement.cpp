#include "CamMovement.h"
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>

float sensitivity = 0.06f;
float Xoffset = 0;
float Yoffset = 0;

float fov = 60.0f;
float LastX = 400, LastY = 300;
float deltaTime, lastFrame;
glm::vec3 Destination; 
float Wwidth = 1980, Wheight = 1080;
bool canMove = true;

Camera::Camera()
{
    window = glfwCreateWindow(1980, 1080, "Scene", NULL, NULL);
    glfwMakeContextCurrent(window);
}
Camera::~Camera()
{
    glfwDestroyWindow(window);
}
GLFWwindow* Camera::CamReturnContextWindow()
{
    return window;
}
void Camera::Handle_Input(glm::vec3& cameraPos, glm::vec3& cameraFront, glm::vec3& cameraUp) {
    float currentFrame = glfwGetTime();
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;
    if (deltaTime > 0.05) // Cap deltaTime to avoid large spikes
        deltaTime = 0.05;
    bool IsShiftting = glfwGetKey(window, GLFW_KEY_RIGHT_SHIFT) == GLFW_PRESS;
    float cameraSpeed =  (IsShiftting ? speed : Shiftspeed ) * static_cast<float>(deltaTime);
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        cameraPos += glm::normalize(Destination) * cameraSpeed;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        cameraPos -= glm::normalize(Destination) * cameraSpeed;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        cameraPos -= glm::normalize(glm::cross(Destination, cameraUp)) * cameraSpeed;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        cameraPos += glm::normalize(glm::cross(Destination, cameraUp)) * cameraSpeed;
    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
        cameraPos += cameraUp * cameraSpeed;
    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
        cameraPos -= cameraUp * cameraSpeed;
    if(canMove)
     MouseCamMovement();
    

}

void Camera::UpdateMatrices(unsigned int viewMatLocation, unsigned int projectionLocation, glm::vec3 cameraPos, glm::vec3 cameraFront, glm::vec3 cameraUp) {
    glm::mat4 viewMat = glm::lookAt(cameraPos, cameraPos + Destination, cameraUp);
    

    glm::mat4 projectionMat = glm::perspective(glm::radians(fov), Wwidth / Wheight, near, far);

    glUniformMatrix4fv(viewMatLocation, 1, GL_FALSE, glm::value_ptr(viewMat));
    glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, glm::value_ptr(projectionMat));
}
void Camera::MouseCamMovement()
{
    /*glfwSetMouseButtonCallback(window, MouseButtonCallBack);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);*/
    //glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, ScrollCallBack);
    glfwSetWindowSizeCallback(window, WindowSizeCallBack);
    
}
glm::vec3 Camera::CameraForward()
{
    return glm::normalize(Destination);
}
void WindowSizeCallBack(GLFWwindow* window, int width, int height)
{
    Wwidth = static_cast<float>(width);
    Wheight = static_cast<float>(height);

    glViewport(0,0,Wwidth,Wheight);
}
void MouseButtonCallBack(GLFWwindow* window, int button , int action , int mods)
{
    if (button == GLFW_MOUSE_BUTTON_RIGHT)
    {
        if (action == GLFW_PRESS)
            canMove = true;
        else if (action == GLFW_RELEASE)
            canMove = false;
    }
}
void ScrollCallBack(GLFWwindow* window, double ScrollX, double ScrollY)
{
    fov -= ScrollY;
    sensitivity = fov / 500;
    if (fov < 1.0f)
    {
        fov = 1.0f;
    }
    else if (fov > 60.0f)
    {
        fov = 60.0f;
    }
}

void Camera::mouse_callback(GLFWwindow* window, double MouseX, double MouseY)
{
    float invertedY = 1080.0f - MouseY;
    DrawCircle(30, 100, glm::vec2(static_cast<float>(MouseX), invertedY));

    if (firstframe)
    {
        firstframe = false;
        LastY = MouseY;
        LastX = MouseX;
    }

    float X = MouseX - LastX;
    float Y = LastY - MouseY;
    LastX = MouseX; 
    LastY = MouseY;

    X *= sensitivity;
    Y *= sensitivity;
    Xoffset += X;
    Yoffset += Y;

    if (Yoffset > 89.0f)
    {
        Yoffset = 89.0f;
    }
    else if (Yoffset < -89.0f)
    {
        Yoffset = -89.0f;
    }
        
    Destination.x = glm::cos(glm::radians(Xoffset));
    Destination.y = glm::sin(glm::radians(Yoffset));
    Destination.z = glm::sin(glm::radians(Xoffset)) * glm::cos(glm::radians(Yoffset));

    Destination = glm::normalize(Destination);
}

// Cercle to follow the mouse

void DrawCircle(int steps,int radius, glm::vec2 Pos)
{
    float x = 0, y = 0;

    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(Pos.x,Pos.y);
    
    for (int i = 0; i < steps; i++)
    {
        float angle = glm::two_pi<float>() * i / steps;
        x = Pos.x + radius * glm::cos(angle);
        x = Pos.y + radius * glm::sin(angle);    
        glVertex2f(x, y);
    }
    glEnd();
}