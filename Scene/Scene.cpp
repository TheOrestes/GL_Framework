
#include "Scene.h"
#include "../ObjectSystem/DirectionalLightObject.h"
#include "../ObjectSystem/PointLightObject.h"
#include "../ObjectSystem/StaticObject.h"
#include "../ShaderEngine/GLSLShader.h"
#include "../Camera/Camera.h"
#include "../ObjectSystem/LightsManager.h"


//////////////////////////////////////////////////////////////////////////////////////////
Scene::Scene()
{
	/*m_pCube1 = new GLCube(glm::vec4(1,0,0,0));
	m_pCube2 = new GLCube(glm::vec4(0,1,0,0));
	m_pCube3 = new GLCube(glm::vec4(0,0,1,0));*/

	m_pOmni1 = new PointLightObject(glm::vec4(1,0,0,1));
	m_pOmni2 = new PointLightObject(glm::vec4(0,1,1,1));
	m_pOmni3 = new PointLightObject(glm::vec4(0,1,0,1));

	m_pObj1 = new StaticObject("Data/BadFatGuy.fbx", "UberShader");
	m_pObj2 = new StaticObject("Data/Plane.fbx", "Diffuse");
	m_pReflection = new StaticObject("Data/ponyBadGuy.fbx", "Outline");

	m_pGlobalDirectional = new DirectionalLightObject(glm::vec4(1,1,1,1));
}

//////////////////////////////////////////////////////////////////////////////////////////
Scene::~Scene()
{
	Kill();
}

//////////////////////////////////////////////////////////////////////////////////////////
void	Scene::Init()
{
	m_pObj1->Init();
	m_pObj1->SetPosition(glm::vec3(0,0,0));
	m_pObj1->SetScale(glm::vec3(0.1));

	m_pObj2->Init();
	m_pObj2->SetScale(glm::vec3(1));

	m_pReflection->Init();
	m_pReflection->SetPosition(glm::vec3(0,0,0));
	m_pReflection->SetScale(glm::vec3(0.101,0.101,0.101));

	m_pOmni1->Init();
	m_pOmni1->SetLightPosition(glm::vec3(7,3,0));
	m_pOmni1->SetLightIntensity(5);
	LightsManager::getInstance()->GatherPointLights(static_cast<GameObject*>(m_pOmni1));

	m_pOmni2->Init();
	m_pOmni2->SetLightPosition(glm::vec3(-7,3,0));
	m_pOmni2->SetLightIntensity(2);
	LightsManager::getInstance()->GatherPointLights(static_cast<GameObject*>(m_pOmni2));

	m_pOmni3->Init();
	m_pOmni3->SetLightPosition(glm::vec3(0,5,7));
	m_pOmni3->SetLightIntensity(2);
	m_pOmni3->SetLightRadius(1);
	LightsManager::getInstance()->GatherPointLights(static_cast<GameObject*>(m_pOmni3));

	m_pGlobalDirectional->SetLightDirection(glm::vec3(0,-1,-1));
	m_pGlobalDirectional->SetLightColor(glm::vec4(1,1,0,1));
	m_pGlobalDirectional->SetLightIntensity(0.5);
	LightsManager::getInstance()->GatherDirectionalLights(static_cast<GameObject*>(m_pGlobalDirectional));
}

//////////////////////////////////////////////////////////////////////////////////////////
void	Scene::Update(float dt)
{
	/*m_pCube1->Update(dt);
	m_pCube2->Update(dt);
	m_pCube3->Update(dt);*/

	m_pOmni1->Update(dt);
	m_pOmni2->Update(dt);
	m_pOmni3->Update(dt);

	m_pObj1->Update(dt);
	//m_pObj2->Update(dt);
}

//////////////////////////////////////////////////////////////////////////////////////////
void	Scene::Render()
{
	/*m_pCube1->Render();
	m_pCube2->Render();
	m_pCube3->Render();*/

	m_pOmni1->Render();
	m_pOmni2->Render();
	m_pOmni3->Render();

	m_pObj1->Render();
	m_pObj2->Render();

	//StencilRender();
}

//////////////////////////////////////////////////////////////////////////////////////////
void	Scene::StencilRender()
{
	// Draw object normally first ... 
	/*m_pObj1->Render();

	glEnable(GL_STENCIL_TEST);

		glStencilFunc(GL_ALWAYS, 1, 0xFF);
		glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
		glStencilMask(0xFF);
		glDepthMask(GL_FALSE);
		glClear(GL_STENCIL_BUFFER_BIT);

		m_pObj2->Render();

		glStencilFunc(GL_EQUAL, 1, 0xFF);
		glStencilMask(0x00);
		glDepthMask(GL_TRUE);

		m_pReflection->Render();

	glDisable(GL_STENCIL_TEST);*/


	// Outline

	// draw plane
	m_pObj2->Render();

	glEnable(GL_STENCIL_TEST);
		
		glStencilFunc(GL_ALWAYS, 1, 0xFF);
		glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
		glStencilMask(0xFF);

		glClear(GL_STENCIL_BUFFER_BIT);

		m_pObj1->Render();

		glStencilFunc(GL_NOTEQUAL, 1, 0XFF);
		glStencilMask(0x00);
		glDisable(GL_DEPTH_TEST);

		m_pReflection->Render();
		glEnable(GL_DEPTH_TEST);

	glDisable(GL_STENCIL_TEST);
}

//////////////////////////////////////////////////////////////////////////////////////////
void	Scene::Kill()
{
	/*if(m_pCube1)
	{
		delete m_pCube1;
		m_pCube1 = nullptr;
	}

	if (m_pCube2)
	{
		delete m_pCube2;
		m_pCube2 = nullptr;
	}
	
	if (m_pCube3)
	{
		delete m_pCube3;
		m_pCube3 = nullptr;
	}*/

	delete m_pOmni1;
	delete m_pOmni2;
	delete m_pOmni3;

	delete m_pGlobalDirectional;

	delete m_pObj1;
	delete m_pObj2;
	delete m_pReflection;
}