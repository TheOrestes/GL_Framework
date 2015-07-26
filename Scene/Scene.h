
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

	PointLightObject*		m_pOmni1;
	PointLightObject*		m_pOmni2;
	PointLightObject*		m_pOmni3;

	StaticObject*			m_pSphere;
	StaticObject*			m_pCube;
	StaticObject*			m_pCylinder;
	StaticObject*			m_pTorus;
	StaticObject*			m_pPlane;
	StaticObject*			m_pAxis;

	DirectionalLightObject*	m_pGlobalDirectional;
};