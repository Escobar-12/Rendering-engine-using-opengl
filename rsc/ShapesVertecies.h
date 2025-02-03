#pragma once

// Cube

float cubevertices[] = {
    // Positions           // Colors         // Texture Coords
    // Back face
    -0.5f, -0.5f, -0.5f,   1.0f, 0.0f, 0.0f,  0.0f, 0.0f, // Bottom-left
     0.5f, -0.5f, -0.5f,   1.0f, 0.0f, 0.0f,  1.0f, 0.0f, // Bottom-right
     0.5f,  0.5f, -0.5f,   1.0f, 0.0f, 0.0f,  1.0f, 1.0f, // Top-right
    -0.5f,  0.5f, -0.5f,   1.0f, 0.0f, 0.0f,  0.0f, 1.0f, // Top-left

    // Front face
    -0.5f, -0.5f,  0.5f,   0.0f, 1.0f, 0.0f,  0.0f, 0.0f, // Bottom-left
     0.5f, -0.5f,  0.5f,   0.0f, 1.0f, 0.0f,  1.0f, 0.0f, // Bottom-right
     0.5f,  0.5f,  0.5f,   0.0f, 1.0f, 0.0f,  1.0f, 1.0f, // Top-right
    -0.5f,  0.5f,  0.5f,   0.0f, 1.0f, 0.0f,  0.0f, 1.0f, // Top-left

    // Left face
    -0.5f, -0.5f, -0.5f,   0.0f, 0.0f, 1.0f,  0.0f, 0.0f, // Bottom-back
    -0.5f, -0.5f,  0.5f,   0.0f, 0.0f, 1.0f,  1.0f, 0.0f, // Bottom-front
    -0.5f,  0.5f,  0.5f,   0.0f, 0.0f, 1.0f,  1.0f, 1.0f, // Top-front
    -0.5f,  0.5f, -0.5f,   0.0f, 0.0f, 1.0f,  0.0f, 1.0f, // Top-back

    // Right face
     0.5f, -0.5f, -0.5f,   1.0f, 1.0f, 0.0f,  0.0f, 0.0f, // Bottom-back
     0.5f, -0.5f,  0.5f,   1.0f, 1.0f, 0.0f,  1.0f, 0.0f, // Bottom-front
     0.5f,  0.5f,  0.5f,   1.0f, 1.0f, 0.0f,  1.0f, 1.0f, // Top-front
     0.5f,  0.5f, -0.5f,   1.0f, 1.0f, 0.0f,  0.0f, 1.0f, // Top-back

    // Bottom face
    -0.5f, -0.5f, -0.5f,   1.0f, 0.0f, 1.0f,  0.0f, 0.0f, // Back-left
     0.5f, -0.5f, -0.5f,   1.0f, 0.0f, 1.0f,  1.0f, 0.0f, // Back-right
     0.5f, -0.5f,  0.5f,   1.0f, 0.0f, 1.0f,  1.0f, 1.0f, // Front-right
    -0.5f, -0.5f,  0.5f,   1.0f, 0.0f, 1.0f,  0.0f, 1.0f, // Front-left

    // Top face
    -0.5f,  0.5f, -0.5f,   0.0f, 1.0f, 1.0f,  0.0f, 0.0f, // Back-left
     0.5f,  0.5f, -0.5f,   0.0f, 1.0f, 1.0f,  1.0f, 0.0f, // Back-right
     0.5f,  0.5f,  0.5f,   0.0f, 1.0f, 1.0f,  1.0f, 1.0f, // Front-right
    -0.5f,  0.5f,  0.5f,   0.0f, 1.0f, 1.0f,  0.0f, 1.0f  // Front-left
};
unsigned int cubeindices[] = {
    // Back face
    0, 1, 2,
    2, 3, 0,

    // Front face
    4, 5, 6,
    6, 7, 4,

    // Left face
    8, 9, 10,
    10, 11, 8,

    // Right face
    12, 13, 14,
    14, 15, 12,

    // Bottom face
    16, 17, 18,
    18, 19, 16,

    // Top face
    20, 21, 22,
    22, 23, 20
};

// ______________________________________
// Pyramid
const float Pyramidvertices[] = {
    // Base vertices (forming two triangles for the square)
    -0.5f, -0.5f, -0.5f,    0.0f, -1.0f,  0.0f,   0.0f, 0.0f, // 0: Bottom-left-front
     0.5f, -0.5f, -0.5f,    0.0f, -1.0f,  0.0f,   1.0f, 0.0f, // 1: Bottom-right-front
     0.5f, -0.5f,  0.5f,    0.0f, -1.0f,  0.0f,   1.0f, 1.0f, // 2: Bottom-right-back
    -0.5f, -0.5f,  0.5f,    0.0f, -1.0f,  0.0f,   0.0f, 1.0f, // 3: Bottom-left-back

    // Front face
    -0.5f, -0.5f, -0.5f,    0.0f,  0.707f, -0.707f,  0.0f, 0.0f, // 4: Bottom-left-front
     0.5f, -0.5f, -0.5f,    0.0f,  0.707f, -0.707f,  1.0f, 0.0f, // 5: Bottom-right-front
     0.0f,  0.5f,  0.0f,    0.0f,  0.707f, -0.707f,  0.5f, 1.0f, // 6: Apex for front face

    // Right face
     0.5f, -0.5f, -0.5f,    0.707f, 0.707f,  0.0f,   0.0f, 0.0f, // 7: Bottom-right-front
     0.5f, -0.5f,  0.5f,    0.707f, 0.707f,  0.0f,   1.0f, 0.0f, // 8: Bottom-right-back
     0.0f,  0.5f,  0.0f,    0.707f, 0.707f,  0.0f,   0.5f, 1.0f, // 9: Apex for right face

    // Back face
     0.5f, -0.5f,  0.5f,    0.0f,  0.707f,  0.707f,  0.0f, 0.0f, // 10: Bottom-right-back
    -0.5f, -0.5f,  0.5f,    0.0f,  0.707f,  0.707f,   1.0f, 0.0f, // 11: Bottom-left-back
     0.0f,  0.5f,  0.0f,    0.0f,  0.707f,  0.707f,   0.5f, 1.0f, // 12: Apex for back face

    // Left face
    -0.5f, -0.5f,  0.5f,   -0.707f, 0.707f,  0.0f,   0.0f, 0.0f, // 13: Bottom-left-back
    -0.5f, -0.5f, -0.5f,   -0.707f, 0.707f,  0.0f,   1.0f, 0.0f, // 14: Bottom-left-front
     0.0f,  0.5f,  0.0f,   -0.707f, 0.707f,  0.0f,   0.5f, 1.0f  // 15: Apex for left face
};


const unsigned int Pyramidindices[] = {
    // Base (should be defined in counter-clockwise order when viewed from below)
    0, 1, 2, // First triangle of the base
    0, 2, 3, // Second triangle of the base

    // Front face
    4, 6, 5, 

    // Right face
    7, 9, 8, 

    // Back face
    10, 12, 11, 

    // Left face
    13, 15, 14  
};

float Quadvertices[] = {
    // Positions         // Normals          // Texture Coords
    -0.5f, -0.5f, 0.0f,   0.0f,  0.0f, 1.0f,   0.0f, 0.0f,  // Bottom-left
     0.5f, -0.5f, 0.0f,   0.0f,  0.0f, 1.0f,   1.0f, 0.0f,  // Bottom-right
     0.5f,  0.5f, 0.0f,   0.0f,  0.0f, 1.0f,   1.0f, 1.0f,  // Top-right
    -0.5f,  0.5f, 0.0f,   0.0f,  0.0f, 1.0f,   0.0f, 1.0f   // Top-left
};
unsigned int Quadindices[] = {
    0, 1, 2,  // First triangle
    0, 2, 3   // Second triangle
};
const float Tranparentshit[] = {
    // positions         // texture Coords (swapped y coordinates because texture is flipped upside down)
    0.0f,  0.5f,  0.0f,  0.0f,  0.0f,
    0.0f, -0.5f,  0.0f,  0.0f,  1.0f,
    1.0f, -0.5f,  0.0f,  1.0f,  1.0f,

    0.0f,  0.5f,  0.0f,  0.0f,  0.0f,
    1.0f, -0.5f,  0.0f,  1.0f,  1.0f,
    1.0f,  0.5f,  0.0f,  1.0f,  0.0f
};
const unsigned int indices[] = {
    0, 1, 2,  // First triangle
    3, 4, 5   // Second triangle
};
