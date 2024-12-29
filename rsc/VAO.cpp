#include "VAO.h"
#include <iostream> // For debugging purposes (optional)

VertexArrayObject::VertexArrayObject() {
    glGenVertexArrays(1, &id); // Correctly initialize the member variable
    std::cout << "VAO created with ID: " << id << std::endl; // Debugging output
}

VertexArrayObject::~VertexArrayObject() {
    glDeleteVertexArrays(1, &id);
    std::cout << "VAO with ID " << id << " deleted." << std::endl; // Debugging output
}

void VertexArrayObject::BindVAO() const {
    glBindVertexArray(id);
}

void VertexArrayObject::UnbindVAO() const {
    glBindVertexArray(0);
}

void VertexArrayObject::AddAttribute(const VertexAttribute& VertexAttribute) {
    Attributes.push_back(VertexAttribute); // Store attributes for later setup
}

void VertexArrayObject::SetUpAttribute() {
    BindVAO();
    for (const auto& attr : Attributes) {
        glEnableVertexAttribArray(attr.index);
        glVertexAttribPointer(attr.index, attr.count, attr.type, attr.IsNormalized, attr.StrideSize, attr.Step);
    }
    UnbindVAO();
}

unsigned int VertexArrayObject::getID() const {
    return id; // Return the VAO ID
}
