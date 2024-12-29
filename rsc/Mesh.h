#pragma once

#include <string>
#include <iostream>
#include <glm.hpp>
#include "VAO.h"
#include "VBO.h"
#include "IBO.h"
#include "TextureSetter.h"

#define BufferOffset(Offset) ((void*)(Offset))

struct Material {
glm::vec3 Diffuse;
glm::vec3 Specular;
glm::vec3 Ambient;
float Shininess;
};

struct Texture {
    unsigned int id;
    std::string type;
    std::string path;
};

struct Vertex {
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 texcoord;
};



class Mesh {
public:
    Mesh(const float vertices[], size_t vertexCount, const unsigned int indices[], size_t indexCount, std::vector<Texture> MeshTextures);
    void Draw(unsigned int PGM);
    std::vector<Texture> Mtextures;
    unsigned int VAO;
    bool texturesInitialized = false;  // Add a flag to track texture initialization
private:
    int diffuseCount = 1;
    int specularCount = 1;
    unsigned int DrawingSize;
    unsigned int VBO, EBO;

};
