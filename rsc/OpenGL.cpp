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
unsigned int SelectProgram;
unsigned int transProgram;

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
unsigned int InitShader(const char* VShader, const char* FShader)
{
    // compiling the shaders 
    unsigned int VertexShader = SCompiler(GL_VERTEX_SHADER, VShader);
    unsigned int FragmentShader = SCompiler(GL_FRAGMENT_SHADER, FShader);

    unsigned int Program =  glCreateProgram();
    glAttachShader(Program,VertexShader);
    glAttachShader(Program,FragmentShader);
    glLinkProgram(Program);
    glBindVertexArray(0);
    glDeleteShader(VertexShader);
    glDeleteShader(FragmentShader);
    return Program;
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

    for (int i = 0; i < 2; i++)
    {
        glm::mat4x4 ModleMat(1.0);
        ModleMat = glm::translate(ModleMat, twoPos[i]);
        ModleMat = glm::translate(ModleMat, glm::vec3(sin(i * (float)glfwGetTime()), 0, cos(i * (float)glfwGetTime())));
        ModleMat = glm::rotate(ModleMat, i + 3 * (float)glfwGetTime(), glm::vec3(0, 1, 0));
        glUniformMatrix4fv(UModel, 1, GL_FALSE, glm::value_ptr(ModleMat));
    }
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
    unsigned int UModel = glGetUniformLocation(SelectProgram, "Model");
    
    glm::mat4x4 ModleMat(1.0);
    ModleMat = glm::translate(ModleMat, LightPos);
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

void OutLineSelect()
{

}
void enableStencilForOutline() {
    glEnable(GL_STENCIL_TEST);
    glStencilFunc(GL_ALWAYS, 1, 0xFF); // Always pass stencil test
    glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE); // Replace stencil buffer value
    glStencilMask(0xFF); // Allow writing to stencil buffer
}

void enableStencilForDrawing() {
    glStencilFunc(GL_NOTEQUAL, 1, 0xFF); // Draw where stencil is not 1
    glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
    glStencilMask(0x00); // Prevent writing to stencil buffer
    glDisable(GL_DEPTH_TEST); // Disable depth to avoid z-fighting
}

void resetStencil() {
    glStencilFunc(GL_ALWAYS, 1, 0xFF); // Always pass stencil test
    glStencilMask(0xFF); // Allow stencil buffer writes
    glEnable(GL_DEPTH_TEST); // Re-enable depth test
}


int main() {
    glfwInit();

    Camera SceneCam;
    glewInit();


    Program = InitShader(VShader,FShader);
    transProgram = InitShader(TransparentVS, TransparentFS);

    InitLight();
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_STENCIL_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    
// Create a TextureSetter object and set the texture
    TextureSetter text;

    // Set the texture for the mesh
    //unsigned int textureId = text.TextureSet("backpack/grass.png", true);
    unsigned int TransId = text.TextureSet("backpack/blending_transparent_window.png", true);

    // Now create a vector of textures with the correct texture ID
    //std::vector<Texture> textures = {
    //    {textureId, "diffuse", "backpack/grass.png"}
    //};
    std::vector<Texture> TransTexture ={
        {TransId, "diffuse","backpack/blending_transparent_window.png"}
    };

    // Create the Mesh instance
    //Mesh r(Pyramidvertices, sizeof(Pyramidvertices) / sizeof(Pyramidvertices[0]), Pyramidindices, sizeof(Pyramidindices) / sizeof(Pyramidindices[0]), textures);

    // Load the model
    Model BackPack("backpack/backpack.obj");
    // drawing wire form
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    Mesh trans(Pyramidvertices, sizeof(Pyramidvertices)/ sizeof(Pyramidvertices[0]), Pyramidindices, sizeof(Pyramidindices)/sizeof(Pyramidindices[0]) , TransTexture);
    
    glUseProgram(Program);

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

    glUseProgram(transProgram);
    unsigned int TModel = glGetUniformLocation(transProgram, "Model");
    glUniformMatrix4fv(TModel, 1, GL_FALSE, glm::value_ptr(ModleMat));

    unsigned int TransUView = glGetUniformLocation(transProgram, "View");
    unsigned int TransUProjection = glGetUniformLocation(transProgram, "Projection");

    // light cube
    glUseProgram(SelectProgram);
    unsigned int LightUView = glGetUniformLocation(SelectProgram, "View");
    unsigned int LightUProjection = glGetUniformLocation(SelectProgram, "Projection");
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
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
        glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
        // perform depth test but do not write to the depth buffer(read-only)
        //glDepthMask(GL_FALSE);
        //glStencilMask(0xFF);
 
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
        glUseProgram(SelectProgram);
        SceneCam.UpdateMatrices(LightUView, LightUProjection,cameraPos,cameraFront,cameraUp);
        CameraForward = SceneCam.CameraForward();
        glUseProgram(Program);
        // Pyramid
        glUniform3fv(UViewPos, 1, glm::value_ptr(cameraPos));
        SceneCam.UpdateMatrices(UView, UProjection, cameraPos, cameraFront, cameraUp);
        glUseProgram(transProgram);
        SceneCam.UpdateMatrices(TransUView, TransUProjection, cameraPos, cameraFront, cameraUp);

        
        

        enableStencilForOutline();
        BackPack.Draw(Program);
        enableStencilForDrawing();
        glUniform1i(glGetUniformLocation(Program, "Selected"), 1);
        BackPack.Draw(Program);
        glUniform1i(glGetUniformLocation(Program, "Selected"), 0);
        resetStencil();
        
        trans.Draw(transProgram);

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
    //delete(text);
    glfwDestroyWindow(SceneCam.CamReturnContextWindow());
    glfwTerminate();
}
