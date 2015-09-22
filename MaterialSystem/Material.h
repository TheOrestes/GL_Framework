
#pragma once

#include "glm/glm.hpp"

struct Material
{
	// default constructor
	Material()
	{
		m_color = glm::vec4(1.0,0.0,0.0,1.0);
		m_colSpecular = glm::vec4(1);
		m_colRoughness = glm::vec4(glm::vec3(0.1),1);
		m_colReflection = glm::vec4(0.2);

		m_fMetallic = 1.0f;
	}

	// parameterized constructor
	Material(const glm::vec4& _Color, const glm::vec4& _specCol, 
			 const glm::vec4& _roughCol, const glm::vec4& _reflCol, float _metal)
	{
		m_color = _Color;
		m_colSpecular = _specCol;
		m_colRoughness = _roughCol;
		m_colReflection = _reflCol;

		m_fMetallic = _metal;
	}

	// Material properties
	glm::vec4	m_color;
	glm::vec4	m_colSpecular;
	glm::vec4	m_colRoughness;
	glm::vec4	m_colReflection;
	float		m_fMetallic;
};

