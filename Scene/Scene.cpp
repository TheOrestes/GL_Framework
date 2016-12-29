
#include "Scene.h"
#include "../ObjectSystem/DirectionalLightObject.h"
#include "../ObjectSystem/PointLightObject.h"
#include "../ObjectSystem/StaticObject.h"
#include "../ShaderEngine/GLSLShader.h"
#include "../Camera/Camera.h"
#include "../ObjectSystem/LightsManager.h"
#include "../Renderables/GLSkybox.h"
#include "nfd.h"

//////////////////////////////////////////////////////////////////////////////////////////
Scene::Scene()
{
	m_pOmni1 = nullptr;
	m_pMesh = nullptr; 

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
	StaticObjectData dataMesh;
	dataMesh.name = "AngelStatue";
	dataMesh.path = "Data/Models/angelStatue/angelStatue.fbx";
	dataMesh.shader = "UberShader";
	dataMesh.position = glm::vec3(0,5,0);
	dataMesh.angle = 0.0f;
	dataMesh.rotation = glm::vec3(0,1,0);                                        
	dataMesh.scale = 1.0f;
	dataMesh.showBBox = false;

	m_pMesh = new StaticObject(dataMesh);
	m_pMesh->Init();

	GLSkybox::getInstance().Init();

	m_pOmni1 =  new PointLightObject(glm::vec4(5,5,0,1));
	m_pOmni1->Init();
	m_pOmni1->SetLightPosition(glm::vec3(6,15,0));
	m_pOmni1->SetLightIntensity(1);
	//LightsManager::getInstance()->GatherPointLights(static_cast<GameObject*>(m_pOmni1));

	m_pGlobalDirectional->m_vecLightDirection = glm::vec3(2,-1,-1);
	m_pGlobalDirectional->m_vecLightColor = glm::vec4(1,1,1,1);
	m_pGlobalDirectional->m_fIntensity = 1.0f;
	LightsManager::getInstance()->GatherDirectionalLights(static_cast<GameObject*>(m_pGlobalDirectional));

	InitUI();
}


//////////////////////////////////////////////////////////////////////////////////////////
void TW_CALL UpdateHDRI(void* data)
{
	nfdchar_t* outPath = nullptr;
	nfdresult_t result = NFD_OpenDialog("hdr,exr", nullptr, &outPath);

	if (result == NFD_OKAY)
	{
		GLSkybox::getInstance().SetHDRIPath((std::string)outPath);
	}
}

//////////////////////////////////////////////////////////////////////////////////////////
void	Scene::InitUI()
{
	m_pSceneUIBar = TwNewBar("Scene");

	TwAddVarRW(m_pSceneUIBar, "DirectionalLight", TW_TYPE_DIR3F, glm::value_ptr(m_pGlobalDirectional->m_vecLightDirection), "label='Light Direction'");
	TwAddVarRW(m_pSceneUIBar, "LightColor", TW_TYPE_COLOR4F, glm::value_ptr(m_pGlobalDirectional->m_vecLightColor), "label='Light Color'");
	TwAddVarRW(m_pSceneUIBar, "LightIntensity", TW_TYPE_FLOAT, &(m_pGlobalDirectional->m_fIntensity),"label='Intensity' min=0 max=10 step=0.1");
	TwAddButton(m_pSceneUIBar, "UpdateHDRI", UpdateHDRI, nullptr, "label='Change HDRI'");
}

//////////////////////////////////////////////////////////////////////////////////////////
void	Scene::Update(float dt)
{
	//m_pOmni1->Update(dt);
	GLSkybox::getInstance().Update(dt);
	m_pMesh->Update(dt);
}

//////////////////////////////////////////////////////////////////////////////////////////
void	Scene::Render()
{
	//m_pOmni1->Render();
	GLSkybox::getInstance().Render();
	m_pMesh->Render();
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
	delete m_pGlobalDirectional;
	delete m_pMesh;
}