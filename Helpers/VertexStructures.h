
#pragma once

#include "glew.h"
#include <string>
#include "glm/glm.hpp"

#include "assimp/Importer.hpp"
#include "assimp/postprocess.h"
#include "assimp/scene.h"


//////////////////////////////////////////////////////////////////////////////////////////
struct VertexP
{
	VertexP() : position(0.0f){}
	VertexP(glm::vec3 _p) : position(_p){}

	glm::vec3 position;
};

//////////////////////////////////////////////////////////////////////////////////////////
struct VertexPT
{
	VertexPT() : 
		position(0.0f),
		uv(0.0f){}

	VertexPT(const glm::vec3& _p, const glm::vec2& _uv ) :
		position(_p),
		uv(_uv){}

	glm::vec3 position;
	glm::vec2 uv;
};

//////////////////////////////////////////////////////////////////////////////////////////
struct VertexPC
{
	VertexPC() : 
		position(0.0f),
		color(1.0f){}

	VertexPC(const glm::vec3& _p, const glm::vec4& _c) : 
		position(_p),
		color(_c) {}

	glm::vec3	position;
	glm::vec4	color;
};

//////////////////////////////////////////////////////////////////////////////////////////
struct VertexPN
{
	VertexPN() : 
		position(0.0f),
		normal(1.0f){}

	VertexPN(const glm::vec3& _p, const glm::vec3& _n) : 
		position(_p),
		normal(_n) {}

	glm::vec3	position;
	glm::vec3	normal;
};

//////////////////////////////////////////////////////////////////////////////////////////
struct VertexPCT
{
	VertexPCT() : 
		position(0.0f),
		color(1.0f),
		uv(0.0f){}

	VertexPCT(const glm::vec3& _p, const glm::vec4& _c, const glm::vec2& _uv) : 
		position(_p),
		color(_c),
		uv(_uv){}

	glm::vec3	position;
	glm::vec4	color;
	glm::vec2	uv;
};

//////////////////////////////////////////////////////////////////////////////////////////
struct VertexPNT
{
	VertexPNT() :
		position(0.0f),
		normal(1.0f),
		texcoord(0.0f){}

	VertexPNT(const glm::vec3& _p, const glm::vec3& _n, const glm::vec2& _uv) :
		position(_p),
		normal(_n),
		texcoord(_uv){}

	glm::vec3	position;
	glm::vec3	normal;
	glm::vec2	texcoord;
};

//////////////////////////////////////////////////////////////////////////////////////////
struct VertexPNTBT
{
	VertexPNTBT() :
		position(0.0f),
		normal(1.0f),
		tangent(0.0f),
		binormal(0.0f),
		texcoord(0.0f){}

	VertexPNTBT(const glm::vec3& _p, const glm::vec3& _n, const glm::vec3& _t, const glm::vec3& _b, const glm::vec2& _uv) :
		position(_p),
		normal(_n),
		tangent(_t),
		binormal(_b),
		texcoord(_uv){}

	glm::vec3	position;
	glm::vec3	normal;
	glm::vec3   tangent;
	glm::vec3   binormal;
	glm::vec2	texcoord;
};

//////////////////////////////////////////////////////////////////////////////////////////
struct BBox
{
	// logical stuff...
	float min_x;
	float max_x;
	float min_y;
	float max_y;
	float min_z;
	float max_z;

	glm::vec3 bbSize;
	glm::vec3 bbCenter;

	// Rendering stuff...
	GLuint vbo;
	GLuint ibo;
	GLuint vao;

	VertexP bbVertices[8];
	GLuint  bbIndices[16];
};

//////////////////////////////////////////////////////////////////////////////////////////
struct StaticObjectData
{
	StaticObjectData()
	{
		name.clear();
		path.clear();
		shader.clear();

		position  = glm::vec3(0);
		rotation  = glm::vec3(0,1,0);
		angle	  = 0.0f;
		scale     = glm::vec3(1);

		showBBox  = false;
	}

	std::string name;
	std::string path;
	std::string shader;

	glm::vec3	position;
	glm::vec3	rotation;
	float		angle;
	glm::vec3	scale;

	bool		showBBox;
};

