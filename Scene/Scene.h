
#pragma once

#include <vector>

class DirectionalLightObject;
class PointLightObject;
class StaticObject;
class GLSkybox;

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

	PointLightObject*		m_pWhiteOmni1;
	PointLightObject*		m_pOmni2;
	PointLightObject*		m_pOmni3;

	StaticObject*			m_pObj1;

	DirectionalLightObject*	m_pGlobalDirectional;
};