
#pragma once

#include <vector>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "AntTweakBar.h"

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
	void			InitUI();

private:

	PointLightObject*		m_pOmni1;

	StaticObject*			m_pMesh;
	StaticObject*			m_pSkySphere;

	DirectionalLightObject*	m_pGlobalDirectional;

	TwBar*					m_pSceneUIBar;
};