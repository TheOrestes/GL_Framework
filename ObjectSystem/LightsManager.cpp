
#include "LightsManager.h"
#include "GameObject.h"
#include "DirectionalLightObject.h"
#include "PointLightObject.h"

//////////////////////////////////////////////////////////////////////////////////////////
LightsManager::LightsManager()
{
	m_iNumPointLights = 0;
	m_iNumDirLights = 0;

	m_vecDirLights.clear();
	m_vecPointLights.clear();
}

//////////////////////////////////////////////////////////////////////////////////////////
LightsManager::~LightsManager()
{

}

//////////////////////////////////////////////////////////////////////////////////////////
void LightsManager::GatherPointLights(GameObject* obj)
{
	m_vecPointLights.push_back(static_cast<PointLightObject*> (obj));
	m_iNumPointLights = m_vecPointLights.size();
}

//////////////////////////////////////////////////////////////////////////////////////////
PointLightObject* LightsManager::GetPointLight(int id)
{
	return m_vecPointLights.at(id);
}

//////////////////////////////////////////////////////////////////////////////////////////
void LightsManager::GatherDirectionalLights( GameObject* obj )
{
	m_vecDirLights.push_back(static_cast<DirectionalLightObject*>(obj));
	m_iNumDirLights = m_vecDirLights.size();
}

//////////////////////////////////////////////////////////////////////////////////////////
DirectionalLightObject* LightsManager::GetDirectionalLight( int id )
{
	return m_vecDirLights.at(id);
}
