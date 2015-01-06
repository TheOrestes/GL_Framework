
#include "Scene.h"
#include "../Helpers/AssimpLoader.h"

//////////////////////////////////////////////////////////////////////////////////////////
Scene::Scene()
{
	m_pCube1 = new GLCube(glm::vec4(1,0,0,0));
	m_pCube2 = new GLCube(glm::vec4(0,1,0,0));
	m_pCube3 = new GLCube(glm::vec4(0,0,1,0));
	m_pMesh = new GLMesh();
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
	//m_pCube1->Init();

	m_pCube2->Init();
	m_pCube2->SetPosition(glm::vec3(5,0,5));
	
	m_pCube3->Init();
	m_pCube3->SetPosition(glm::vec3(4,0,-4));

	bool status = AssimpLoader::getInstance().LoadAssimpMesh("Data/teapot.dae", &m_pMesh);
	if(status)
	{
		m_pMesh->Init();
	}
}

//////////////////////////////////////////////////////////////////////////////////////////
void	Scene::Update(float dt)
{
	m_pCube1->Update(dt);
	m_pCube2->Update(dt);
	m_pCube3->Update(dt);

	m_pMesh->Update(dt);
}

//////////////////////////////////////////////////////////////////////////////////////////
void	Scene::Render()
{
	//m_pCube1->Render();
	m_pCube2->Render();
	//m_pCube3->Render();

	m_pMesh->Render();
}

//////////////////////////////////////////////////////////////////////////////////////////
void	Scene::Kill()
{
	if(m_pCube1)
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
	}
	
	if (m_pMesh)
	{
		delete m_pMesh;
		m_pMesh = nullptr;
	}
}