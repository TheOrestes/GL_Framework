
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

private:

	PointLightObject*		m_pOmni1;
	StaticObject*			m_pObj1;
};