#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"


#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "Myimgui.h"
#include <assimp/config.h>

#include <mat4x4.hpp>
#include <glm.hpp>
#include <gtc/type_ptr.hpp>
#include <gtc/matrix_transform.hpp>

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <cstring>

#include "VAO.h"
#include "VBO.h"
#include "IBO.h"

#include "CamMovement.h"

#include "TextureSetter.h"
#include "Shaders.h"
#include "LightVertecies.h"
#include "ShapesVertecies.h"
#include <filesystem>
#include "Mesh.h"
#include "Model.h"

using namespace std;

#define BufferrOffset(Offset) ((void *)(Offset))


glm::vec3 cameraPos;
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
unsigned int Program;
unsigned int LightProgram;

glm::mat4x4 ViewMat(1.0f);


glm::vec3 cubePositions[] = {
    glm::vec3(0.0f,  0.0f,  0.0f),
    glm::vec3(2.0f,  5.0f, -15.0f),
    glm::vec3(-1.5f, -2.2f, -2.5f),
    glm::vec3(-3.8f, -2.0f, -12.3f),
    glm::vec3(2.4f, -0.4f, -3.5f),
    glm::vec3(-1.7f,  3.0f, -7.5f),
    glm::vec3(1.3f, -2.0f, -2.5f),
    glm::vec3(1.5f,  2.0f, -2.5f),
    glm::vec3(1.5f,  0.2f, -1.5f),
    glm::vec3(-1.3f,  1.0f, -1.5f)
};
glm::vec3 twoPos[] =
{
     glm::vec3(0.0f,  0.0f,  0.0f),
     glm::vec3(1.5f,  0.2f, -1.5f),
};

glm::vec3 LightPos = glm::vec3(-1.5f, 2.2f, -2.5f);
glm::vec3 LightcubePos;


unsigned int SCompiler(GLenum type, const char* SharedSource)
{
    unsigned int id = glCreateShader(type);
    glShaderSource(id, 1, &SharedSource,NULL);
    glCompileShader(id);
    // Check for compilation errors
    int success;
    glGetShaderiv(id, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        char infoLog[512];
        glGetShaderInfoLog(id, sizeof(infoLog), NULL, infoLog);
        std::cerr << "ERROR: Shader Compilation Failed ("
            << (type == GL_VERTEX_SHADER ? "Vertex" : "Fragment")
            << ")\n"
            << infoLog << std::endl;
        glDeleteShader(id);
        return 0; // Return 0 to indicate failure
    }

    return id;
}
void InitShader()
{
    // compiling the shaders 
    unsigned int VertexShader = SCompiler(GL_VERTEX_SHADER, VShader);
    unsigned int FragmentShader = SCompiler(GL_FRAGMENT_SHADER, FShader);

    Program =  glCreateProgram();
    glAttachShader(Program,VertexShader);
    glAttachShader(Program,FragmentShader);
    glLinkProgram(Program);
    glBindVertexArray(0);
    glDeleteShader(VertexShader);
    glDeleteShader(FragmentShader);
}
void InitLight()
{
    // compiling the shaders 

    unsigned int VertexShader = SCompiler(GL_VERTEX_SHADER, LightVShader);
    unsigned int FragmentShader = SCompiler(GL_FRAGMENT_SHADER, LightFShader);

    LightProgram = glCreateProgram();
    glAttachShader(LightProgram, VertexShader);
    glAttachShader(LightProgram, FragmentShader);
    glLinkProgram(LightProgram);
    glBindVertexArray(0);
    glDeleteShader(VertexShader);
    glDeleteShader(FragmentShader);
}

float offset = 0;


void displayPyramid()
{
    unsigned int UModel = glGetUniformLocation(Program, "Model");
    glm::mat4x4 ModleMat(1.0);
    glUniformMatrix4fv(UModel,1,GL_FALSE,glm::value_ptr(ModleMat));
}
void displaycube()
{
    unsigned int UModel = glGetUniformLocation(Program, "Model");

        glm::mat4x4 ModleMat(1.0);
        ModleMat = glm::translate(ModleMat, cubePositions[2]);
        glUniformMatrix4fv(UModel, 1, GL_FALSE, glm::value_ptr(ModleMat));
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, nullptr);
}
void DisplayLight()
{
    unsigned int UModel = glGetUniformLocation(LightProgram, "Model");
    
    LightcubePos =  LightPos;
    glm::mat4x4 ModleMat(1.0);
    ModleMat = glm::scale(ModleMat,glm::vec3(0.25,0.25,0.25));
    ModleMat = glm::translate(ModleMat, LightcubePos);
    //ModleMat = glm::translate(ModleMat, cubePos);
    glUniformMatrix4fv(UModel, 1, GL_FALSE, glm::value_ptr(ModleMat));
    
    glDrawArrays(GL_TRIANGLES,0, 36);
    
}

void dynamicResize(float val)
{
    unsigned int UModel = glGetUniformLocation(Program, "Model");
    glm::mat4x4 ModleMat(1.0);
    ModleMat = glm::scale(ModleMat,glm::vec3(val, val, val));
    glUniformMatrix4fv(UModel, 1, GL_FALSE, glm::value_ptr(ModleMat));
}

int main() {
    glfwInit();

    Camera SceneCam;
    glewInit();


    InitShader();
    InitLight();
    glDisable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);

    
    
    
    glUseProgram(Program);

    
    // Pyramid
    TextureSetter T0("container2.png", true);
    TextureSetter T4("container2_specular.png", true);

    //Texture MeshT0 = { T0, "diffuse"};
    //Texture MeshT1 = { T4, "specular"};

    //Mesh pyramidMesh(Tvertices, sizeof(Tvertices), Tindices, sizeof(Tindices), {MeshT0,MeshT1});

    TextureSetter T1("backpack/diffuse.png", true);
    TextureSetter T3("backpack/specular.png", true);

    Model BackPack("backpack/backpack.obj");
    // drawing wire form
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    TextureSetter T5("container2.png", true);
    TextureSetter T6("container2_specular.png", true);



    //Camera Movments
    unsigned int UView = glGetUniformLocation(Program, "View");
    unsigned int UProjection = glGetUniformLocation(Program, "Projection");
    // Material Settings
    unsigned int UMaterialA = glGetUniformLocation(Program, "material.ambient");
    unsigned int UMaterialShine = glGetUniformLocation(Program, "material.shininess");
    
    glUniform1f(UMaterialA,1);
    glUniform1f(UMaterialShine, 32);

    // Light Settings
    // DL
    unsigned int ULightC = glGetUniformLocation(Program, "Dlight.LightColor");
    unsigned int ULightA = glGetUniformLocation(Program, "Dlight.ambient");
    unsigned int ULightD = glGetUniformLocation(Program, "Dlight.diffuse");
    unsigned int ULightS = glGetUniformLocation(Program, "Dlight.specular");
    unsigned int UDLightDir = glGetUniformLocation(Program, "Dlight.direction");
    glUniform3f(ULightC, 1.0, 1.0, 1.0);
    glUniform3f(ULightA, 0.4, 0.4, 0.4);
    glUniform3f(ULightD, 0.8, 0.8, 0.8);
    glUniform3f(ULightS, 1.0f, 1.0f, 1.0f);
    glUniform3f(UDLightDir, 0, -100, 0);

    //SL
    unsigned int USLightInCutOff = glGetUniformLocation(Program, "Slight.IncutOffangle");
    unsigned int USLightOutCutOff = glGetUniformLocation(Program, "Slight.OutcutOffangle");
    glUniform1f(USLightInCutOff, glm::cos(glm::radians(12.5f)));
    glUniform1f(USLightOutCutOff, glm::cos(glm::radians(26.0f)));
    unsigned int USLightPos = glGetUniformLocation(Program, "Slight.position");
    unsigned int USLightDir = glGetUniformLocation(Program, "Slight.direction");

    unsigned int UViewPos = glGetUniformLocation(Program, "ViewPos");

    unsigned int USLightconstant = glGetUniformLocation(Program, "Slight.constant");
    glUniform1f(USLightconstant, 1.0f);
    unsigned int USLightlinear = glGetUniformLocation(Program, "Slight.linear");
    glUniform1f(USLightlinear, 0.09f);
    unsigned int USLightquadratic = glGetUniformLocation(Program, "Slight.quadratic");
    glUniform1f(USLightquadratic, 0.032f);

   // set near and far planes 
    glUniform1f(glGetUniformLocation(Program, "nearPPlane"), SceneCam.near);
    glUniform1f(glGetUniformLocation(Program, "farPPlane"), SceneCam.far);


    // p model mat4x4
    unsigned int UModel = glGetUniformLocation(Program, "Model");
    glm::mat4x4 ModleMat(1.0);

    glUniformMatrix4fv(UModel, 1, GL_FALSE, glm::value_ptr(ModleMat));

 
    // light cube
    glUseProgram(LightProgram);
    unsigned int LightUView = glGetUniformLocation(LightProgram, "View");
    unsigned int LightUProjection = glGetUniformLocation(LightProgram, "Projection");
    //glUniform1i(UText2,1);
    //glUniform1i(UText, 1);
    //Texture1.TextureBind(1);


    glm::vec3 CameraForward;

    // --------------------- imgui

    
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
    //io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
    io.ConfigWindowsResizeFromEdges = true;
    io.ConfigWindowsMoveFromTitleBarOnly = true;
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(SceneCam.CamReturnContextWindow(), true);
    ImGui_ImplOpenGL3_Init("#version 430");
    IAMAGUI GUI(SceneCam.CamReturnContextWindow());

    //-------------------------------
    
    float val = 0;
    float val2 = 0;
    float scale = 1;
    while (!glfwWindowShouldClose(SceneCam.CamReturnContextWindow())) {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        // perform depth test but do not write to the depth buffer(read-only)
        //glDepthMask(GL_FALSE);
 
 
        // -----------------imgui
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        if ((!io.WantCaptureMouse && !io.WantCaptureKeyboard && (io.MouseDown[1])))
        {
            SceneCam.mouse_callback(SceneCam.CamReturnContextWindow(), io.MousePos.x, io.MousePos.y);
            SceneCam.Handle_Input(cameraPos, cameraFront, cameraUp);
            glfwSetInputMode(SceneCam.CamReturnContextWindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        }
        else
        {
            glfwSetInputMode(SceneCam.CamReturnContextWindow(), GLFW_CURSOR, GLFW_CURSOR_NORMAL);
            SceneCam.firstframe = true;
        }
        // _----------------------------
 

 
        // Cam
        glUseProgram(Program);
        SceneCam.UpdateMatrices(LightUView, LightUProjection,cameraPos,cameraFront,cameraUp);
        CameraForward = SceneCam.CameraForward();
        
        // Pyramid
        glUniform3fv(UViewPos, 1, glm::value_ptr(cameraPos));
        SceneCam.UpdateMatrices(UView, UProjection, cameraPos, cameraFront, cameraUp);
        displayPyramid();
        //BackPack.Draw(Program);
        BackPack.Draw(Program);

        //pyramidMesh.Draw(LightProgram);
        
           

        //-------------------------------
         
          GUI.DockingSpaceFunc(&GUI.DockingSpaceEnable);
        // drawing with imgui
        ImGui::Begin("Inspector");
        ImGui::SliderFloat("value", &val, 0, 5);
        
        ImGui::Begin("Inspector2");
        
        ImGui::SliderFloat("value", &val2, 0, 100);
        ImGui::End();
        ImGui::End();
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
        {
            // Update and Render additional Platform Windows
            ImGui::UpdatePlatformWindows();
            ImGui::RenderPlatformWindowsDefault();

            // TODO for OpenGL: restore current GL context.
            glfwMakeContextCurrent(SceneCam.CamReturnContextWindow());
        }

        glfwSwapBuffers(SceneCam.CamReturnContextWindow());
        glfwPollEvents();
    }

    // terminating imgui
    //GUI.~IAMAGUI();

    glDeleteProgram(Program);

    glfwDestroyWindow(SceneCam.CamReturnContextWindow());
    glfwTerminate();
}
