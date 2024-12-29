#pragma once
#include <gl/glew.h>
#include <iostream>
#include <string>
#include "stb_image.h"

class TextureSetter
{
private:
	
	int height, width, nchannels;
	unsigned int TextureId;
	unsigned char* TextureData;
public:
	TextureSetter(const char* FileName, bool Reversed);
	~TextureSetter();
	void TextureBind(unsigned int Slote = 0);
	void TextureUnbind();
	unsigned int TextureGetId();
};