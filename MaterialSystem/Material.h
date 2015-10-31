
#pragma once

#include "glm/glm.hpp"

struct Material
{
	// default constructor
	Material()
	{
		m_colAlbedo = glm::vec4(1.0,0.0,0.0,1.0);
		m_colRoughness = glm::vec4(glm::vec3(0.1), 1);
		m_colMetallic = glm::vec4(glm::vec3(0.5), 1);
	}

	// parameterized constructor
	Material(const glm::vec4& _Color, const glm::vec4& _roughCol, 
		     const glm::vec4& _metlCol)
	{
		m_colAlbedo = _Color;
		m_colRoughness = _roughCol;
		m_colMetallic = _metlCol;
	}

	// Material properties
	glm::vec4	m_colAlbedo;
	glm::vec4	m_colRoughness;
	glm::vec4	m_colMetallic;
};

