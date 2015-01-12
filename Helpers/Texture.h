
#pragma once

#include "GL/glew.h"
#include "SOIL.h"

#include <string>

class Texture
{
public:
	Texture(GLenum textureTarget, const std::string& filename);

	bool			Load();
	void			Bind(GLenum textureTarget);

private:
	std::string		m_fileName;
	GLenum			m_textureTarget;
	GLuint			m_textureObject;
};