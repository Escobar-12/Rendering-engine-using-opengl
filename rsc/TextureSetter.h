#pragma once
#include <gl/glew.h>
#include <iostream>
#include <string>
#include "stb_image.h"


class TextureSetter {
public:
    TextureSetter();  // Default constructor
    unsigned int TextureSet(const char* FileName, bool Reversed);  // Method to set texture

};
