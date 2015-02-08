
#include "StaticObject.h"
#include "../ShaderEngine/GLSLShader.h"
#include "../Renderables/Model.h"

//////////////////////////////////////////////////////////////////////////////////////////
StaticObject::StaticObject()
{
	m_strShader.clear();
	m_strPath.clear();

	m_pShader = nullptr;
	m_pModel = nullptr;
}

//////////////////////////////////////////////////////////////////////////////////////////
StaticObject::StaticObject(const std::string& path, const std::string& shader)
	:	m_strPath(path),
		m_strShader(shader)
{
	m_pShader = nullptr;
	m_pModel = nullptr;
}

//////////////////////////////////////////////////////////////////////////////////////////
StaticObject::~StaticObject()
{
	Kill();
}

//////////////////////////////////////////////////////////////////////////////////////////
void StaticObject::Init()
{
	// initialize shader
	if(m_strShader.empty())
	{
		m_pShader = new GLSLShader("Shaders/vsUberShader.glsl", "Shaders/psUberShader.glsl");
	}
	else
	{
		std::string vertShaderPath;
		vertShaderPath = "Shaders/vs" + m_strShader + ".glsl";

		std::string fragShaderPath;
		fragShaderPath = "Shaders/ps" + m_strShader + ".glsl";

		m_pShader = new GLSLShader(vertShaderPath, fragShaderPath);
	}

	// initialize model
	m_pModel = new Model(m_strPath);
}

//////////////////////////////////////////////////////////////////////////////////////////
void StaticObject::Kill()
{
	delete m_pShader;
	delete m_pModel;
}

//////////////////////////////////////////////////////////////////////////////////////////
void StaticObject::Update( float dt )
{
	static float angle = 0;
	angle += dt;

	m_fAngle = angle;
}

//////////////////////////////////////////////////////////////////////////////////////////
void StaticObject::Render()
{
	glm::mat4 model;
	model = glm::translate(model, m_vecPosition); 
	model = glm::rotate(model, m_fAngle, m_vecRotation);
	model = glm::scale(model, m_vecScale);	

	m_pModel->Render(m_pShader, model);
}

//////////////////////////////////////////////////////////////////////////////////////////
void StaticObject::SetPosition( const glm::vec3& pos )
{
	m_vecPosition = pos;
}

//////////////////////////////////////////////////////////////////////////////////////////
void StaticObject::SetRotation( const glm::vec3& axis, float angle )
{
	m_vecRotation = axis;
	m_fAngle = angle;
}

//////////////////////////////////////////////////////////////////////////////////////////
void StaticObject::SetScale( const glm::vec3& sc )
{
	m_vecScale = sc;
}


