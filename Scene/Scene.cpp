
#include "Scene.h"
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
	m_pObj2 = new StaticObject("Data/UnitCube.fbx", "UberShader");
	//m_pObj3 = new GameObject(3, "Sphere", "Data/duck.dae");
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
	m_pObj1->SetPosition(glm::vec3(0,-2,0));
	m_pObj1->SetScale(glm::vec3(0.1));

	m_pObj2->Init();
	m_pObj2->SetPosition(glm::vec3(-10,0,-200));
	m_pObj2->SetScale(glm::vec3(5));

	m_pOmni1->Init();
	m_pOmni1->SetLightPosition(glm::vec3(5,3,0));
	m_pOmni1->SetLightIntensity(5);
	LightsManager::getInstance()->GatherPointLights(static_cast<GameObject*>(m_pOmni1));

	m_pOmni2->Init();
	m_pOmni2->SetLightPosition(glm::vec3(-5,0,0));
	m_pOmni2->SetLightIntensity(2);
	LightsManager::getInstance()->GatherPointLights(static_cast<GameObject*>(m_pOmni2));

	m_pOmni3->Init();
	m_pOmni3->SetLightPosition(glm::vec3(0,0,5));
	m_pOmni3->SetLightIntensity(2);
	m_pOmni3->SetLightRadius(1);
	LightsManager::getInstance()->GatherPointLights(static_cast<GameObject*>(m_pOmni3));
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
	m_pObj2->Update(dt);
	//m_pObj3->Update(dt);
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
	//m_pObj2->Render();
	//m_pObj3->Render();
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

	delete m_pObj1;
	delete m_pObj2;
	//delete m_pObj3;
}