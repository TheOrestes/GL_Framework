
#pragma once

#include <vector>
#include "../Renderables/GLCube.h"
#include "../Renderables/GLCubeNormal.h"

class GameObject;

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
	GLCube*			m_pCube1;
	GLCube*			m_pCube2;
	GLCube*			m_pCube3;

	GameObject*		m_pObj1;
	GameObject*		m_pObj2;
	GameObject*		m_pObj3;
};