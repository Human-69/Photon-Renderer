#include "Photon.h";
#include <stb_image/stb_image.h>

Texture::Texture(const std::string path)
	:ID(0), filePath(path), width(0), height(0), BPP(0), localBuffer(nullptr)
{
	
	stbi_set_flip_vertically_on_load(1);
	localBuffer = stbi_load(path.c_str(), &width, &height, &BPP, 4);
	

	glGenTextures(1, &ID);
	glBindTexture(GL_TEXTURE_2D, ID);
	

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, localBuffer);
	glBindTexture(GL_TEXTURE_2D, 0);

}
Texture::~Texture(){}

void Texture::Bind(unsigned int slot)
{
	glActiveTexture(GL_TEXTURE0 + slot);
	glBindTexture(GL_TEXTURE_2D, ID);
}
void Texture::UnBind()
{
	glBindTexture(GL_TEXTURE_2D, 0);
}

int Texture::GetWidth() { return width; }
int Texture::GetHeight() { return height; }