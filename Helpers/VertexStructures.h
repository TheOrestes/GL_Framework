
#pragma once

#include <string>
#include "glm/glm.hpp"

//////////////////////////////////////////////////////////////////////////////////////////
struct VertexP
{
	VertexP() : position(0.0f){}
	VertexP(glm::vec3 _p) : position(_p){}

	glm::vec3 position;
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

	VertexPNT(const glm::vec3& _p, const glm::vec3& _n, const glm::vec3& _uv) :
		position(_p),
		normal(_n),
		texcoord(_uv){}

	glm::vec3	position;
	glm::vec3	normal;
	glm::vec2	texcoord;
};

