
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

	// Getter
	inline glm::vec3	GetLightDirection(){ return m_vecLightDirection; }
	inline glm::vec4	GetLightColor(){ return m_vecLightColor; }
	inline float		GetLightIntensity(){ return m_fIntensity; }

	// Setter
	void				SetLightDirection(const glm::vec3& dir);
	void				SetLightColor(const glm::vec4& col);
	void				SetLightIntensity(float intensity);

private:
	glm::vec3			m_vecLightDirection;
	glm::vec4			m_vecLightColor;
	float				m_fIntensity;
};
