
#include <iostream>
#include <string>
#include <vector>
#include "TextureManager.h"
#include "Helper.h"
#include "LogManager.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

//////////////////////////////////////////////////////////////////////////////////////////
TextureManager::TextureManager()
{
}

//////////////////////////////////////////////////////////////////////////////////////////
TextureManager::~TextureManager()
{
}

//////////////////////////////////////////////////////////////////////////////////////////
GLint TextureManager::LoadTextureFromFile(const std::string& path)
{
	int width, height, bpp;
	void* data;

	GLuint textureID;
	glGenTextures(1, &textureID);

	if (stbi_is_hdr(path.c_str()))
	{
		stbi_set_flip_vertically_on_load(1);
		data = stbi_loadf(path.c_str(), &width, &height, &bpp, 0);

		if (data != nullptr)
		{
			LogManager::getInstance().WriteToConsole(LOG_INFO, "TextureManager", "HDRI texture loaded");
			// Assign texture to ID
			glBindTexture(GL_TEXTURE_2D, textureID);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB32F, width, height, 0, GL_RGB, GL_FLOAT, (void*)data);
			glGenerateMipmap(GL_TEXTURE_2D);
			stbi_image_free(data);

			// Parameters
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glGenerateMipmap(GL_TEXTURE_2D);

			glBindTexture(GL_TEXTURE_2D, 0);
		}
		else
		{
			textureID = -1;
		}
		
	}
	else
	{
		stbi_set_flip_vertically_on_load(1);
		data = stbi_load(path.c_str(), &width, &height, &bpp, 0);

		if (data != nullptr)
		{
			LogManager::getInstance().WriteToConsole(LOG_INFO, "TextureManager", "Texture loaded");

			// Assign texture to ID
			glBindTexture(GL_TEXTURE_2D, textureID);

			if(bpp>3)
				glTexImage2D(GL_TEXTURE_2D, 0, GL_SRGB_ALPHA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, (void*)data);
			else
				glTexImage2D(GL_TEXTURE_2D, 0, GL_SRGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, (void*)data);

			glGenerateMipmap(GL_TEXTURE_2D);
			stbi_image_free(data);

			// Parameters
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glGenerateMipmap(GL_TEXTURE_2D);

			glBindTexture(GL_TEXTURE_2D, 0);
		}
		else
		{
			textureID = -1;
		}
	}

	return textureID;
}

//////////////////////////////////////////////////////////////////////////////////////////
GLint TextureManager::LoadCubemapFromFile(const std::string& dir)
{
	// we work on the assumption that folder contains specifically named textures
	// forming a cubemap!
	// Do some book keeping first by storing textures in the folder into a vector
	std::vector<std::string> vecCubemapTextures;
	vecCubemapTextures.push_back(dir + '/' + "posx.jpg");
	vecCubemapTextures.push_back(dir + '/' + "negx.jpg");
	vecCubemapTextures.push_back(dir + '/' + "posy.jpg");
	vecCubemapTextures.push_back(dir + '/' + "negy.jpg");
	vecCubemapTextures.push_back(dir + '/' + "posz.jpg");
	vecCubemapTextures.push_back(dir + '/' + "negz.jpg");

	GLuint cubemapID;
	glGenTextures(1, &cubemapID);
	glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapID);

		for (int i = 0 ; i<vecCubemapTextures.size() ; i++)
		{
			unsigned char* data;
			int width, height;
			data = stbi_load(vecCubemapTextures[i].c_str(), &width, &height, nullptr, 0);
			if (data != nullptr)
			{
				glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_SRGB_ALPHA, width, height, 0, GL_BGRA, GL_UNSIGNED_BYTE, (void*)data);
			}
			stbi_image_free(data);
		}

		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
		glGenerateMipmap(GL_TEXTURE_CUBE_MAP);

	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

	return cubemapID;
}

//////////////////////////////////////////////////////////////////////////////////////////
GLint TextureManager::LoadHDRICubemapFromFile( const std::string& name )
{
	// we work on the assumption that folder contains specifically named textures
	// forming a cubemap!
	// Do some book keeping first by storing textures in the folder into a vector
	std::vector<std::string> vecCubemapTextures;
	vecCubemapTextures.push_back(name + '/' + "right.tif");
	vecCubemapTextures.push_back(name + '/' + "left.tif");
	vecCubemapTextures.push_back(name + '/' + "bottom.tif");
	vecCubemapTextures.push_back(name + '/' + "top.tif");
	vecCubemapTextures.push_back(name + '/' + "front.tif");
	vecCubemapTextures.push_back(name + '/' + "back.tif");

	GLuint cubemapID;
	glGenTextures(1, &cubemapID);
	glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapID);

	for (int i = 0 ; i<vecCubemapTextures.size() ; i++)
	{
		unsigned char* data;
		int width, height;
		data = stbi_load(vecCubemapTextures[i].c_str(), &width, &height, nullptr, 0);
		
		if (data != nullptr)
		{
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB9_E5, width, height, 0, GL_RGB, GL_FLOAT, (void*)data);
			stbi_image_free(data);
		}
	}

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glGenerateMipmap(GL_TEXTURE_CUBE_MAP);

	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

	return cubemapID;
}



