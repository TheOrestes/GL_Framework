
#pragma once

#include "GameObject.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#include "../Renderables/GLLight.h"

class PointLightObject : public GameObject
{
public:
	PointLightObject();
	PointLightObject(const glm::vec4& color);
	virtual ~PointLightObject();

	virtual void		Init();
	virtual void		Kill();
	virtual void		Update(float dt);
	virtual void		Render();

	// Getter
	inline glm::vec3	GetLightPosition(){ return m_vecLightPosition; }
	inline glm::vec3	GetLightColor(){ return m_vecLightColor; }
	inline float		GetLightIntensity(){ return m_fIntensity; }
	inline float		GetLightRadius(){ return m_fRadius; }

	// Setter
	void				SetLightPosition(const glm::vec3& pos);
	void				SetLightColor(const glm::vec3& col);
	void				SetLightIntensity(float intensity);
	void				SetLightRadius(float radius);

private:
	GLLight*			m_pLightMesh;

	glm::vec3			m_vecLightPosition;
	glm::vec3			m_vecLightColor;
	float				m_fIntensity;
	float				m_fRadius;
};
