
#pragma once

#include "glm/glm.hpp"

struct Material
{
	// default constructor
	Material()
	{
		m_colAlbedo = glm::vec4(1.0,0.0,0.0,1.0);
		m_colSpecular = glm::vec4(glm::vec3(1), 1);
		m_colEmissive = glm::vec4(glm::vec3(0.5), 1);
	}

	// parameterized constructor
	Material(const glm::vec4& _Color, const glm::vec4& _specCol, 
		     const glm::vec4& _emissCol)
	{
		m_colAlbedo = _Color;
		m_colSpecular = _specCol;
		m_colEmissive = _emissCol;
	}

	// Material properties
	glm::vec4	m_colAlbedo;
	glm::vec4	m_colSpecular;
	glm::vec4	m_colEmissive;
};

