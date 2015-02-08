
#include "Scene.h"
#include "../ObjectSystem/PointLightObject.h"
#include "../ObjectSystem/StaticObject.h"
#include "../ShaderEngine/GLSLShader.h"
#include "../Camera/Camera.h"


//////////////////////////////////////////////////////////////////////////////////////////
Scene::Scene()
{
	/*m_pCube1 = new GLCube(glm::vec4(1,0,0,0));
	m_pCube2 = new GLCube(glm::vec4(0,1,0,0));
	m_pCube3 = new GLCube(glm::vec4(0,0,1,0));*/

	m_pOmni1 = new PointLightObject();

	m_pObj1 = new StaticObject("Data/UnitTorus.fbx", "UberShader");
	//m_pObj1 = new GameObject(1, "Spirit","Data/UnitTorus.fbx");
	//m_pObj2 = new GameObject(2, "Crate","Data/UnitTorus.fbx");
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
	// Cube
	/*m_pCube1->Init();

	m_pCube2->Init();
	m_pCube2->SetPosition(glm::vec3(5,0,0));
	
	m_pCube3->Init();
	m_pCube3->SetPosition(glm::vec3(-5,0,0));*/

	m_pOmni1->Init();

	m_pObj1->Init();
	m_pObj1->SetPosition(glm::vec3(0,0,0));
	m_pObj1->SetScale(glm::vec3(0.05f, 0.05f, 0.05f));

	/*m_pObj2->Init();
	m_pObj2->SetPosition(glm::vec3(10,0,0));
	m_pObj2->SetScale(glm::vec3(0.05f, 0.05f, 0.05f));

	m_pObj3->Init();
	m_pObj3->SetPosition(glm::vec3(-15,0,0));
	m_pObj3->SetScale(glm::vec3(0.5f, 0.5f, 0.5f));*/
}

//////////////////////////////////////////////////////////////////////////////////////////
void	Scene::Update(float dt)
{
	/*m_pCube1->Update(dt);
	m_pCube2->Update(dt);
	m_pCube3->Update(dt);*/

	m_pOmni1->Update(dt);
	m_pObj1->Update(dt);
	//m_pObj2->Update(dt);
	//m_pObj3->Update(dt);
}

//////////////////////////////////////////////////////////////////////////////////////////
void	Scene::Render()
{
	/*m_pCube1->Render();
	m_pCube2->Render();
	m_pCube3->Render();*/

	m_pOmni1->Render();
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

	delete m_pObj1;
	//delete m_pObj2;
	//delete m_pObj3;
}