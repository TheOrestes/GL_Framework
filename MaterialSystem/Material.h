
#pragma once

#include <string>
#include "glew.h"
#include "glm/glm.hpp"

//////////////////////////////////////////////////////////////////////////////////////////
enum TextureType
{
	TEXTURE_ALBEDO,
	TEXTURE_SPECULAR,
	TEXTURE_NORMAL,
	TEXTURE_HEIGHT,
	TEXTURE_OCCLUSION,
	TEXTURE_EMISSION,
	TEXTURE_MAX_TYPE
};

struct TextureProperty
{
	TextureProperty()
	{
		id = -1;
		name.clear();
		path.clear();
		changed = false;
	}

	GLuint		id;
	std::string name;
	std::string path;
	bool		changed;
	TextureType type;

	inline void setPath(const std::string& _path) { path = _path; };
	inline void setName(const std::string& _name) { name = _name; };
	inline void setID(const int _id) { id = _id; };
	inline void setType(const TextureType _type) { type = _type; }

	std::string getPath()	{ return path; };
	std::string getName()	{ return name; };
	GLuint		getID()		{ return id; };
	TextureType getType()	{ return type; }
	bool		hasChanged() { return changed; }
};

struct Material
{
	// default constructor
	Material()
	{
		m_colAlbedo = glm::vec4(1);
		m_colEmissive = glm::vec4(0, 0, 0, 1);
		m_fMetallic = 0.0f;
		m_fRoughness = 0.5f;
	}

	// Textures
	TextureProperty m_pTexAlbedo;
	TextureProperty m_pTexSpecular;
	TextureProperty m_pTexNormal; 
	TextureProperty m_pTexHeight;
	TextureProperty m_pTexOcclusion;
	TextureProperty m_pTexEmission;

	// Material properties
	glm::vec4	m_colAlbedo;
	glm::vec4	m_colEmissive;
	float		m_fMetallic;	
	float		m_fRoughness;
};

