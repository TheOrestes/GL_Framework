
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

	StaticObject*			m_pMesh;
	StaticObject*			m_pMesh2;
	PointLightObject*		m_pOmni1;
	DirectionalLightObject*	m_pGlobalDirectional;

	TwBar*					m_pSceneUIBar;
};