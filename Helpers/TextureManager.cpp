
#include <iostream>
#include <string>
#include <vector>
#include "TextureManager.h"


//////////////////////////////////////////////////////////////////////////////////////////
TextureManager::TextureManager()
{
	FreeImage_Initialise();
}

//////////////////////////////////////////////////////////////////////////////////////////
TextureManager::~TextureManager()
{
	FreeImage_DeInitialise();
}

//////////////////////////////////////////////////////////////////////////////////////////
FIBITMAP* TextureManager::LoadTextureFromFreeImage( const std::string filepath)
{
	// FreeImage library for loading textures...
	FREE_IMAGE_FORMAT format = FreeImage_GetFileType(filepath.c_str());

	// if image is found, but the format is unknown...
	if(format == FIF_UNKNOWN)
	{
		format = FreeImage_GetFIFFromFilename(filepath.c_str());
		if(!FreeImage_FIFSupportsReading(format))
		{
			std::cout << "FreeImage::Detected Image format cannot be read!" << std::endl;
			return nullptr;
		}
	}

	FIBITMAP* bitmap = FreeImage_Load(format, filepath.c_str());
	
	// Check for floating point image format i.e. HDR
	if (FreeImage_GetImageType(bitmap) != FIT_RGBF)
	{
		bitmap = FreeImage_ConvertTo32Bits(bitmap);
	}
	
	return bitmap;
}

//////////////////////////////////////////////////////////////////////////////////////////
GLint TextureManager::Load2DTextureFromFile(const std::string& path, const std::string& dir)
{
	//Generate texture ID and load texture data 
	std::string filename;
	filename = dir + '/' + path;

	GLuint textureID;
	glGenTextures(1, &textureID);

	FIBITMAP* bitmap32 = LoadTextureFromFreeImage(filename);

	if(bitmap32)
	{
		// grab image width & height
		int width = FreeImage_GetWidth(bitmap32);
		int height = FreeImage_GetHeight(bitmap32);

		// Assign texture to ID
		glBindTexture(GL_TEXTURE_2D, textureID);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_SRGB_ALPHA, width, height, 0, GL_BGRA, GL_UNSIGNED_BYTE, (void*)FreeImage_GetBits(bitmap32));
		glGenerateMipmap(GL_TEXTURE_2D);

		// Parameters
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR );
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glBindTexture(GL_TEXTURE_2D, 0);

		FreeImage_Unload(bitmap32);
	}
	else
	{
		textureID = -1;
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
			FIBITMAP* bitmap = LoadTextureFromFreeImage(vecCubemapTextures[i]);
			if (bitmap)
			{
				glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_SRGB_ALPHA, FreeImage_GetWidth(bitmap), FreeImage_GetHeight(bitmap), 0, GL_BGRA, GL_UNSIGNED_BYTE, (void*)FreeImage_GetBits(bitmap));
				FreeImage_Unload(bitmap);
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
		FIBITMAP* bitmap = LoadTextureFromFreeImage(vecCubemapTextures[i]);
		int width = FreeImage_GetWidth(bitmap);
		int height = FreeImage_GetHeight(bitmap);
		float* bits = (float*)FreeImage_GetBits(bitmap);

		if (bitmap)
		{
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB9_E5, width, height, 
						 0, GL_RGB, GL_FLOAT, bits);
			FreeImage_Unload(bitmap);
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


