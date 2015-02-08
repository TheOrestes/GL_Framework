
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
	virtual ~PointLightObject();

	virtual void		Init();
	virtual void		Kill();
	virtual void		Update(float dt);
	virtual void		Render();

	// Getter
	inline glm::vec3	GetLightPosition(){ return m_vecLightPosition; }
	inline glm::vec4	GetLightColor(){ return m_vecLightColor; }
	inline float		GetLightIntensity(){ return m_fIntensity; }
	inline float		GetLightRadius(){ return m_fRadius; }
	inline int			GetLightAttenuation(){ return m_iAttenuation; }

	// Setter
	void				SetLightPosition(const glm::vec3& pos);
	void				SetLightColor(const glm::vec4& col);
	void				SetLightIntensity(float intensity);
	void				SetLightRadius(float radius);
	void				SetLightAttenuation(int atten);

private:
	GLLight*			m_pLightMesh;

	glm::vec3			m_vecLightPosition;
	glm::vec4			m_vecLightColor;
	float				m_fIntensity;
	float				m_fRadius;
	int					m_iAttenuation;
};
