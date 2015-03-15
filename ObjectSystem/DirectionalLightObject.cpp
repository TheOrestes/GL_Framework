
#include "DirectionalLightObject.h"


//////////////////////////////////////////////////////////////////////////////////////////
DirectionalLightObject::DirectionalLightObject()
{
	m_vecLightDirection = glm::vec3(0,-1,0);
	m_vecLightColor = glm::vec4(1);
	m_fIntensity = 1.0f;
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

//////////////////////////////////////////////////////////////////////////////////////////
void DirectionalLightObject::SetLightDirection(const glm::vec3& dir)
{ 
	m_vecLightDirection = dir; 
}

//////////////////////////////////////////////////////////////////////////////////////////
void DirectionalLightObject::SetLightColor(const glm::vec4& col)
{ 
	m_vecLightColor = col; 
}

//////////////////////////////////////////////////////////////////////////////////////////
void DirectionalLightObject::SetLightIntensity(float intensity)
{ 
	m_fIntensity = intensity; 
}

