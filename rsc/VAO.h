#pragma once
#include <GL/glew.h>
#include <vector>

struct VertexAttribute {
    unsigned int index;
    int count;
    GLenum type;
    GLboolean IsNormalized;
    unsigned int StrideSize;
    const void* Step;
};

class VertexArrayObject {
private:
    unsigned int id; // Correctly scoped member variable for VAO ID
    std::vector<VertexAttribute> Attributes;
    int AttributeCount = 0;

public:
    VertexArrayObject();
    ~VertexArrayObject();

    void BindVAO() const;
    void UnbindVAO() const;

    void AddAttribute(const VertexAttribute& VertexAttribute);
    void SetUpAttribute();
    unsigned int getID() const;
};
