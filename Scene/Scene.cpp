
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
	m_pOmni1 = nullptr;
	/*m_pOmni2 = new PointLightObject(glm::vec4(1,1,0,1));
	m_pOmni3 = new PointLightObject(glm::vec4(0,1,0,1));*/

	//m_pCube   = nullptr;
	m_pSphere = nullptr; 
	/*m_pCylinder = nullptr; 
	m_pTorus = nullptr; 
	m_pPlane = nullptr; 
	m_pAxis = nullptr;*/

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
	//---- Cube
	/*StaticObjectData dataCube;
	dataCube.path = "Data/UnitCube.fbx";
	dataCube.shader = "UberShader";
	dataCube.position = glm::vec3(-4,1,0);
	dataCube.angle = 0.0f;
	dataCube.rotation = glm::vec3(0,1,0);
	dataCube.scale = glm::vec3(1);

	m_pCube = new StaticObject(dataCube);
	m_pCube->Init();

	//---- Plane
	StaticObjectData dataPlane;
	dataPlane.path = "Data/UnitPlane.fbx";
	dataPlane.shader = "UberShader";
	dataPlane.position = glm::vec3(0,0,0);
	dataPlane.angle = 0.0f;
	dataPlane.rotation = glm::vec3(0,1,0);
	dataPlane.scale = glm::vec3(15,1,15);

	m_pPlane = new StaticObject(dataPlane);
	m_pPlane->Init();*/

	//---- Sphere
	StaticObjectData dataSphere;
	dataSphere.path = "Data/nanosuit/Nanosuit.fbx";
	//dataSphere.path = "Data/Arnold.fbx";
	dataSphere.shader = "UberShader";
	dataSphere.position = glm::vec3(0,0,0);
	dataSphere.angle = 0.0f;
	dataSphere.rotation = glm::vec3(0,1,0);                                        
	dataSphere.scale = glm::vec3(1);
	dataSphere.showBBox = false;

	m_pSphere = new StaticObject(dataSphere);
	m_pSphere->Init();

	//---- Cylinder
	/*StaticObjectData dataCylinder;
	dataCylinder.path = "Data/UnitCylinder.fbx";
	dataCylinder.shader = "UberShader";
	dataCylinder.position = glm::vec3(0,1,4);
	dataCylinder.angle = 0.0f;
	dataCylinder.rotation = glm::vec3(0,1,0);
	dataCylinder.scale = glm::vec3(1);
	dataCylinder.showBBox = true;

	m_pCylinder = new StaticObject(dataCylinder);
	m_pCylinder->Init();

	//---- Torus
	StaticObjectData dataTorus;
	dataTorus.path = "Data/UnitTorus.fbx";
	dataTorus.shader = "UberShader";
	dataTorus.position = glm::vec3(0,1,-4);
	dataTorus.angle = 0.0f;
	dataTorus.rotation = glm::vec3(0,1,0);
	dataTorus.scale = glm::vec3(1);
	dataTorus.showBBox = true;

	m_pTorus = new StaticObject(dataTorus);
	m_pTorus->Init();

	//---- Axis
	StaticObjectData dataAxis;
	dataAxis.path = "Data/GL_Axis.fbx";
	dataAxis.shader = "UberShader";
	dataAxis.position = glm::vec3(0,1,0);
	dataAxis.angle = 0.0f;
	dataAxis.rotation = glm::vec3(0,1,0);
	dataAxis.scale = glm::vec3(1);

	m_pAxis = new StaticObject(dataAxis);
	m_pAxis->Init();*/

	m_pOmni1 =  new PointLightObject(glm::vec4(5,5,0,1));
	m_pOmni1->Init();
	m_pOmni1->SetLightPosition(glm::vec3(6,5,0));
	m_pOmni1->SetLightIntensity(1);
	LightsManager::getInstance()->GatherPointLights(static_cast<GameObject*>(m_pOmni1));

	/*m_pOmni2->Init();
	m_pOmni2->SetLightPosition(glm::vec3(-6,5,0));
	m_pOmni2->SetLightIntensity(5);
	LightsManager::getInstance()->GatherPointLights(static_cast<GameObject*>(m_pOmni2));

	m_pOmni3->Init();
	m_pOmni3->SetLightPosition(glm::vec3(0,6,6));
	m_pOmni3->SetLightIntensity(5);
	m_pOmni3->SetLightRadius(1);
	LightsManager::getInstance()->GatherPointLights(static_cast<GameObject*>(m_pOmni3));*/

	m_pGlobalDirectional->SetLightDirection(glm::vec3(0,-1,-1));
	m_pGlobalDirectional->SetLightColor(glm::vec4(1,1,1,1));
	m_pGlobalDirectional->SetLightIntensity(1.5);
	LightsManager::getInstance()->GatherDirectionalLights(static_cast<GameObject*>(m_pGlobalDirectional));

	// initialize skybox
	GLSkybox::getInstance().Init();
}

//////////////////////////////////////////////////////////////////////////////////////////
void	Scene::Update(float dt)
{
	m_pOmni1->Update(dt);
	/*m_pOmni2->Update(dt);
	m_pOmni3->Update(dt);*/

	m_pSphere->Update(dt);
	
	GLSkybox::getInstance().Update(dt);
}

//////////////////////////////////////////////////////////////////////////////////////////
void	Scene::Render()
{
	m_pOmni1->Render();
	/*m_pOmni2->Render();
	m_pOmni3->Render();*/

	//m_pCube->Render();
	//m_pPlane->Render();
	m_pSphere->Render();
	//m_pCylinder->Render();
	//m_pTorus->Render();
	//m_pAxis->Render();*/

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
	delete m_pOmni1;
	/*delete m_pOmni2;
	delete m_pOmni3;*/

	delete m_pGlobalDirectional;

	delete m_pCube;
	/*delete m_pPlane;
	delete m_pSphere;
	delete m_pCylinder;
	delete m_pTorus;
	delete m_pAxis;*/
}