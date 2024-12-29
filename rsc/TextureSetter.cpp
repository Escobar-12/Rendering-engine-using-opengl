#include "TextureSetter.h"

TextureSetter::TextureSetter(const char* FileName, bool Reversed)
{
    glGenTextures(1, &TextureId);  // This will generate a unique texture ID

    glBindTexture(GL_TEXTURE_2D, TextureId);

    stbi_set_flip_vertically_on_load(Reversed);

    // Load image data
    TextureData = stbi_load(FileName, &width, &height, &nchannels, 4);
    if (TextureData) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, TextureData);
        glGenerateMipmap(GL_TEXTURE_2D);  // Generate mipmaps (optional)
    }
    else {
        std::cerr << "Failed to load texture: " << FileName << std::endl;
    }

    // Set texture parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    stbi_image_free(TextureData);  // Free the loaded image data
}

TextureSetter::~TextureSetter()
{
	glDeleteTextures(1, &TextureId);
}
void TextureSetter::TextureBind(unsigned int Slote)
{
	glActiveTexture(GL_TEXTURE0 + Slote);
	glBindTexture(GL_TEXTURE_2D, TextureId);
}
void TextureSetter::TextureUnbind()
{
	glBindTexture(GL_TEXTURE_2D, 0);
}
unsigned int TextureSetter::TextureGetId()
{
	return TextureId;
}