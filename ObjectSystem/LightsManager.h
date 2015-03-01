
#pragma once

#include <vector>

class GameObject;
class PointLightObject;

class LightsManager
{
public:
	static LightsManager* getInstance()
	{
		static LightsManager instance;
		return &instance;
	}

	~LightsManager();

	void							GatherPointLights(GameObject* obj);
	PointLightObject*				GetPointLight(int id);

	inline	int						GetPointlightsCount(){ return m_iNumPointLights; }

private:
	LightsManager();

	int								m_iNumPointLights;
	std::vector<PointLightObject*>	m_vecPointLights;	
};