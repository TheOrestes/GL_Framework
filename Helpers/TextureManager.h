
#pragma once

#include "glew.h"
#include <string>

class TextureManager
{
public:
	static TextureManager& getInstannce()
	{
		static TextureManager instance;
		return instance;
	}

	~TextureManager();

	GLint	LoadTextureFromFile(const std::string& path);
	GLint	LoadCubemapFromFile(const std::string& dir);
	GLint	LoadHDRICubemapFromFile(const std::string& dir);

private:
	TextureManager();
};