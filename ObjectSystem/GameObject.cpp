
#include "GameObject.h"
#include "../Camera/Camera.h"
#include "../ShaderEngine/GLSLShader.h"
#include "../Renderables/Model.h"

//////////////////////////////////////////////////////////////////////////////////////////
GameObject::GameObject(int id, const std::string& name, const std::string& path)
		: m_iID(id),
		  m_strName(name),
		  m_strPath(path),
		  m_vecPosition(0,0,0),
		  m_vecRotation(0,1,0),
		  m_fAngle(0.f),
		  m_vecScale(1,1,1)
{
	m_pShader = nullptr;
	m_pModel = nullptr;
}

//////////////////////////////////////////////////////////////////////////////////////////
GameObject::~GameObject()
{
	Kill();
}

//////////////////////////////////////////////////////////////////////////////////////////
void	GameObject::Init()
{
	// initialize shader
	m_pShader = new GLSLShader("Shaders/vsDiffuse.glsl", "Shaders/psDiffuse.glsl");

	// initialize model
	m_pModel = new Model(m_strPath);
}

//////////////////////////////////////////////////////////////////////////////////////////
void	GameObject::Kill()
{
	delete m_pShader;
	delete m_pModel;
}

//////////////////////////////////////////////////////////////////////////////////////////
void	GameObject::Update(float dt)
{
	// Update this Gameobject's Position, Rotation & scale over here...
	m_fAngle += dt;
}

//////////////////////////////////////////////////////////////////////////////////////////
void	GameObject::Render()
{
	glm::mat4 model;
	model = glm::translate(model, m_vecPosition); 
	model = glm::rotate(model, m_fAngle, m_vecRotation);
	model = glm::scale(model, m_vecScale);	

	m_pModel->Render(m_pShader, model);
}

//////////////////////////////////////////////////////////////////////////////////////////
void GameObject::SetPosition( const glm::vec3& pos )
{
	m_vecPosition = pos;
}

//////////////////////////////////////////////////////////////////////////////////////////
void GameObject::SetRotation( const glm::vec3& axis, float angle )
{
	m_vecRotation = axis;
	m_fAngle = angle;
}

//////////////////////////////////////////////////////////////////////////////////////////
void GameObject::SetScale( const glm::vec3& sc )
{
	m_vecScale = sc;
}
