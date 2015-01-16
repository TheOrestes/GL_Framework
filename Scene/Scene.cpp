
#include "Scene.h"

#include "../Renderables/Model.h"
#include "../ShaderEngine/GLSLShader.h"
#include "../Camera/Camera.h"

//////////////////////////////////////////////////////////////////////////////////////////
GLSLShader* shader;
Model* gameModel; 

//////////////////////////////////////////////////////////////////////////////////////////
Scene::Scene()
{
	m_pCube1 = new GLCube(glm::vec4(1,0,0,0));
	m_pCube2 = new GLCube(glm::vec4(0,1,0,0));
	m_pCube3 = new GLCube(glm::vec4(0,0,1,0));
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

	shader = new GLSLShader("Shaders/vsAmbient.glsl", "Shaders/psAmbient.glsl");
	gameModel = new Model("Data/nanosuit/nanosuit.obj");
}

//////////////////////////////////////////////////////////////////////////////////////////
void	Scene::Update(float dt)
{
	m_pCube1->Update(dt);
	m_pCube2->Update(dt);
	m_pCube3->Update(dt);
}

//////////////////////////////////////////////////////////////////////////////////////////
void	Scene::Render()
{
	//m_pCube1->Render();
	//m_pCube2->Render();
	//m_pCube3->Render();

	shader->Use();
	GLuint shaderID = shader->GetShaderID();

	// Transformation matrices
	glm::mat4 projection = Camera::getInstance().getProjectionMatrix();
	glm::mat4 view = Camera::getInstance().getViewMatrix();

	glUniformMatrix4fv(glGetUniformLocation(shaderID, "matProj"), 1, GL_FALSE, glm::value_ptr(projection));
	glUniformMatrix4fv(glGetUniformLocation(shaderID, "matView"), 1, GL_FALSE, glm::value_ptr(view));

	// Draw the loaded model
	glm::mat4 model;
	model = glm::translate(model, glm::vec3(0.0f, 0, 0.0f)); // Translate it down a bit so it's at the center of the scene
	model = glm::scale(model, glm::vec3(1,1,1));	// It's a bit too big for our scene, so scale it down
	glUniformMatrix4fv(glGetUniformLocation(shaderID, "matWorld"), 1, GL_FALSE, glm::value_ptr(model));
	gameModel->Render(*shader);
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
}