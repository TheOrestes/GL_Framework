
#pragma once

#include <vector>

class PointLightObject;
class StaticObject;

class Scene
{
public:
	Scene();
	~Scene();

	void			Init();
	void			Render();
	void			Update(float dt);
	void			Kill();

	void			GenerateLightList();

private:

	PointLightObject*		m_pOmni1;
	PointLightObject*		m_pOmni2;
	PointLightObject*		m_pOmni3;

	StaticObject*			m_pObj1;
	StaticObject*			m_pObj2;
	StaticObject*			m_pObj3;
};