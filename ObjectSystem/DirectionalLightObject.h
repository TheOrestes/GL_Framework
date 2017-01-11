
#pragma once

#include "GameObject.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#include "../Renderables/GLLight.h"

class DirectionalLightObject : public GameObject
{
public:
	DirectionalLightObject();
	DirectionalLightObject(const glm::vec4& color);
	virtual ~DirectionalLightObject();

	glm::vec3			m_vecLightDirection;
	glm::vec3			m_vecLightColor;
	float				m_fIntensity;
};
