
#include "DirectionalLightObject.h"


//////////////////////////////////////////////////////////////////////////////////////////
DirectionalLightObject::DirectionalLightObject()
{
	m_vecLightDirection = glm::vec3(0,-1,0);
	m_vecLightColor = glm::vec4(1);
	m_fIntensity = 5.0f;
}

//////////////////////////////////////////////////////////////////////////////////////////
DirectionalLightObject::DirectionalLightObject(const glm::vec4& color)
{
	m_vecLightDirection = glm::vec3(0,-1,0);
	m_vecLightColor = color;
	m_fIntensity = 1.0f;
}

//////////////////////////////////////////////////////////////////////////////////////////
DirectionalLightObject::~DirectionalLightObject()
{
	
}



