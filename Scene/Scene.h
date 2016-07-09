
#pragma once

#include <vector>

class DirectionalLightObject;
class PointLightObject;
class StaticObject;

class Scene
{
public:
	Scene();
	~Scene();

	void			Init();
	//void			StencilRender();
	void			Render();
	void			Update(float dt);
	void			Kill();

	void			GenerateLightList();

private:

	PointLightObject*		m_pOmni1;

	StaticObject*			m_pMesh;
	StaticObject*			m_pSkySphere;

	DirectionalLightObject*	m_pGlobalDirectional;
};