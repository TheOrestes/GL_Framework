
#include "Texture.h"

/////////////////////////////////////////////////////////////////////////////////////////
Texture::Texture(GLenum textureTarget, const std::string& filename)
{
	m_textureTarget = textureTarget;
	m_fileName = filename;
}

/////////////////////////////////////////////////////////////////////////////////////////
bool Texture::Load()
{
	int width, height;
	unsigned char* image = SOIL_load_image(m_fileName.c_str(), &width, &height, NULL, 0);

	glGenTextures(1, &m_textureObject);
	glBindTexture(m_textureTarget, m_textureObject);
	glTexImage2D(m_textureTarget, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
	glTexParameterf(m_textureTarget, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(m_textureTarget, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glBindTexture(m_textureTarget, 0);

	return true;
}

/////////////////////////////////////////////////////////////////////////////////////////
void Texture::Bind(GLenum textureTarget)
{
	glActiveTexture(textureTarget);
	glBindTexture(textureTarget, m_textureObject);
}
