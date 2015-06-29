
#include "Scene.h"
#include "../ObjectSystem/DirectionalLightObject.h"
#include "../ObjectSystem/PointLightObject.h"
#include "../ObjectSystem/StaticObject.h"
#include "../ShaderEngine/GLSLShader.h"
#include "../Camera/Camera.h"
#include "../ObjectSystem/LightsManager.h"
#include "../Renderables/GLSkybox.h"

//////////////////////////////////////////////////////////////////////////////////////////
Scene::Scene()
{
	m_pWhiteOmni1 = new PointLightObject(glm::vec4(1,1,1,1));
	m_pOmni2 = new PointLightObject(glm::vec4(1,0,0,1));
	m_pOmni3 = new PointLightObject(glm::vec4(0,1,0,1));

	m_pObj1 = new StaticObject("Data/Chesterfield/Chesterfield.fbx", "UberShader");

	//m_pObj1 = new StaticObject("Data/Chesterfield/Chesterfield.fbx", "UberShader");

	m_pGlobalDirectional = new DirectionalLightObject(glm::vec4(0,1,0,1));
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
	m_pObj1->SetScale(glm::vec3(1));

	m_pWhiteOmni1->Init();
	m_pWhiteOmni1->SetLightPosition(glm::vec3(12,10,0));
	m_pWhiteOmni1->SetLightIntensity(1);
	LightsManager::getInstance()->GatherPointLights(static_cast<GameObject*>(m_pWhiteOmni1));
	
	m_pOmni2->Init();
	m_pOmni2->SetLightPosition(glm::vec3(-3,0,0));
	m_pOmni2->SetLightIntensity(1);
	LightsManager::getInstance()->GatherPointLights(static_cast<GameObject*>(m_pOmni2));
	//
	//m_pOmni3->Init();
	//m_pOmni3->SetLightPosition(glm::vec3(0,10,12));
	//m_pOmni3->SetLightIntensity(5);
	//m_pOmni3->SetLightRadius(1);
	//LightsManager::getInstance()->GatherPointLights(static_cast<GameObject*>(m_pOmni3));

	m_pGlobalDirectional->SetLightDirection(glm::vec3(0,-1,-1));
	m_pGlobalDirectional->SetLightColor(glm::vec4(1,1,1,1));
	m_pGlobalDirectional->SetLightIntensity(0.5);
	LightsManager::getInstance()->GatherDirectionalLights(static_cast<GameObject*>(m_pGlobalDirectional));

	// initialize skybox
	GLSkybox::getInstance().Init();
}

//////////////////////////////////////////////////////////////////////////////////////////
void	Scene::Update(float dt)
{
	m_pWhiteOmni1->Update(dt);
	m_pOmni2->Update(dt);
	//m_pOmni3->Update(dt);

	m_pObj1->Update(dt);
	
	GLSkybox::getInstance().Update(dt);
}

//////////////////////////////////////////////////////////////////////////////////////////
void	Scene::Render()
{
	m_pWhiteOmni1->Render();
	m_pOmni2->Render();
	//m_pOmni3->Render();

	m_pObj1->Render();

	GLSkybox::getInstance().Render();
}

//////////////////////////////////////////////////////////////////////////////////////////
/*void	Scene::StencilRender()
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

	glDisable(GL_STENCIL_TEST);


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
}*/

//////////////////////////////////////////////////////////////////////////////////////////
void	Scene::Kill()
{
	delete m_pWhiteOmni1;
	delete m_pOmni2;
	delete m_pOmni3;

	delete m_pGlobalDirectional;

	delete m_pObj1;
}