#include "Mesh.h"

Mesh::Mesh(const float vertices[], size_t vertexCount, const unsigned int indices[], size_t indexCount, std::vector<Texture> MeshTextures)
    : DrawingSize(static_cast<unsigned int>(indexCount)) {
    // create buffers/arrays
    this->Mtextures = MeshTextures;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertexCount * sizeof(float), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexCount * sizeof(unsigned int), indices, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), BufferOffset(3 * sizeof(float)));
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), BufferOffset(6 * sizeof(float)));
    //// vertex tangent
    //glEnableVertexAttribArray(3);
    //glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Tangent));
    //// vertex bitangent
    //glEnableVertexAttribArray(4);
    //glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Bitangent));
    //// ids
    //glEnableVertexAttribArray(5);
    //glVertexAttribIPointer(5, 4, GL_INT, sizeof(Vertex), (void*)offsetof(Vertex, m_BoneIDs));

    //// weights
    //glEnableVertexAttribArray(6);
    //glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, m_Weights));
    glBindVertexArray(0);
}

void Mesh::Draw(unsigned int PGM) {
    // Only initialize textures once
    if (!texturesInitialized) {
        if (specularCount <= 1 || diffuseCount <= 1) {
            for (size_t i = 0; i < Mtextures.size(); i++) {
                glActiveTexture(GL_TEXTURE0 + i);
                std::string num;
                std::string type;
                if (Mtextures[i].type == "diffuse") {
                    num = std::to_string(diffuseCount++);
                    type = "diffuse";
                }
                else if (Mtextures[i].type == "specular") {
                    num = std::to_string(specularCount++);
                    type = "specular";
                }

                std::string uniformName = "material." + type + num;
                int location = glGetUniformLocation(PGM, uniformName.c_str());
                if (location != -1) {
                    glUniform1i(location, i);
                    std::cout << "Binding texture unit " << i << " to uniform " << uniformName << std::endl;
                }
                std::cout << Mtextures[i].path.c_str() << std::endl;
                glBindTexture(GL_TEXTURE_2D, Mtextures[i].id);
            }
            std::cout << "loading texture" << std::endl;
        }
        texturesInitialized = true;
    }

    // Bind VAO and shader program
    glBindVertexArray(VAO);
    glUseProgram(PGM);

    // Draw the mesh
    if (DrawingSize > 0) {
        glDrawElements(GL_TRIANGLES, DrawingSize, GL_UNSIGNED_INT, nullptr);
    }
    else {
        std::cerr << "Warning: DrawingSize is 0." << std::endl;
    }

    // Unbind VAO
    glBindVertexArray(0);
    glActiveTexture(GL_TEXTURE0);  // Reset active texture
}

