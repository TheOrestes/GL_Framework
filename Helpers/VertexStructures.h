
#pragma once

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

