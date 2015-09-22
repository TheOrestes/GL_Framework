
#pragma once

#include "GL\glew.h"
#include "FreeImage.h"
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

	GLint	Load2DTextureFromFile(const std::string& path, const std::string& dir);
	GLint	LoadCubemapFromFile(const std::string& dir);
	GLint	LoadHDRICubemapFromFile(const std::string& dir);

private:
	TextureManager();
	FIBITMAP*	LoadTextureFromFreeImage(const std::string filepath);
};