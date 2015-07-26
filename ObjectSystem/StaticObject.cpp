
#include "StaticObject.h"
#include "../ShaderEngine/GLSLShader.h"
#include "../Renderables/Model.h"
#include "../MaterialSystem/Material.h"
#include "../Renderables/BBoxCube.h"

//////////////////////////////////////////////////////////////////////////////////////////
StaticObject::StaticObject()
{
	m_strShader.clear();
	m_strPath.clear();

	m_pShader = nullptr;
	m_pModel = nullptr;
	m_pMaterial = nullptr;
	m_pBBoxCube = nullptr;
}

//////////////////////////////////////////////////////////////////////////////////////////
StaticObject::StaticObject(const StaticObjectData& data)
	:	m_strPath(data.path),
		m_strShader(data.shader),
		m_pShader(nullptr),
		m_pModel(nullptr),
		m_pBBoxCube(nullptr)
{
	m_vecPosition  = data.position;
	m_fAngle = data.angle;
	m_vecRotation = data.rotation;
	m_vecScale = data.scale; 
	m_bShowBBox = data.showBBox;
}

//////////////////////////////////////////////////////////////////////////////////////////
StaticObject::~StaticObject()
{
	Kill();
}

//////////////////////////////////////////////////////////////////////////////////////////
void StaticObject::Init()
{
	std::string vertShaderPath;
	vertShaderPath = "Shaders/vs" + m_strShader + ".glsl";

	std::string fragShaderPath;
	fragShaderPath = "Shaders/ps" + m_strShader + ".glsl";

	m_pShader = new GLSLShader(vertShaderPath, fragShaderPath);

	// initialize model
	m_pModel = new Model(m_strPath);

	// initialize material
	m_pMaterial = new Material();
		
	// Initial transformation...
	glm::mat4 model;
	model = glm::translate(model, m_vecPosition); 
	model = glm::rotate(model, m_fAngle, m_vecRotation);
	model = glm::scale(model, m_vecScale);	

	m_matWorld = model;

	// initialize bounding box
	InitBBox(m_pModel->GetVertexPositions());
}

//////////////////////////////////////////////////////////////////////////////////////////
void StaticObject::Kill()
{
	delete m_pShader;
	delete m_pModel;
	delete m_pMaterial;
	delete m_pBBoxCube;
}

//////////////////////////////////////////////////////////////////////////////////////////
void StaticObject::Update( float dt )
{
	static float angle = 0;
	angle += dt;

	m_fAngle = angle;

	glm::mat4 model;
	glm::mat4 matTranslate;
	model = glm::translate(model, m_vecPosition); 
	model = glm::rotate(model, m_fAngle, m_vecRotation);
	model = glm::scale(model, m_vecScale);	

	m_matWorld = model;

	m_pBBoxCube->UpdateBB(m_matWorld, dt);
}

//////////////////////////////////////////////////////////////////////////////////////////
void StaticObject::Render()
{
	m_pModel->Render(m_pShader, model, m_pMaterial);
	
	if (m_bShowBBox)
	{
		m_pBBoxCube->Render(m_matWorld);
	}
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

//////////////////////////////////////////////////////////////////////////////////////////
void StaticObject::SetCentroid( const glm::vec3& center )
{
	m_vecCentroid = center;
}

//////////////////////////////////////////////////////////////////////////////////////////
void StaticObject::ShowBBox( bool flag )
{
	m_bShowBBox = flag;
}

//////////////////////////////////////////////////////////////////////////////////////////
void StaticObject::InitBBox( std::vector<glm::vec3> _vecVertexPositions )
{
	float min_x, min_y, min_z, max_x, max_y, max_z;
	glm::vec3 size, center;

	// setup required iterators...
	std::vector<glm::vec3>::iterator iter = _vecVertexPositions.begin();
	std::vector<glm::vec3>::iterator iterEnd = _vecVertexPositions.end();

	// initialize to first element's value...
	min_x = max_x = (*iter).x;
	min_y = max_y = (*iter).y;
	min_z = max_z = (*iter).z;

	// traverse through all vertices & modify minima & maxima to evaluate
	// final bounds...
	for ( ; iter != iterEnd ; ++iter)
	{
		if ((*iter).x < min_x) { min_x = (*iter).x; }
		if ((*iter).x > max_x) { max_x = (*iter).x; }
		if ((*iter).y < min_y) { min_y = (*iter).y; }
		if ((*iter).y > max_y) { max_y = (*iter).y; }
		if ((*iter).z < min_z) { min_z = (*iter).z; }
		if ((*iter).z > max_z) { max_z = (*iter).z; }
	}

	// calculate size & center...
	size = glm::vec3(max_x-min_x, max_y-min_y, max_z-min_z);
	center = glm::vec3((min_x+max_x)/2, (min_y+max_y)/2, (min_z+max_z)/2);

	// Set object's centroid position...
	SetCentroid(center);

	std::vector<glm::vec3> vecBBVertices;
	vecBBVertices.push_back(glm::vec3(min_x, min_y, min_z));
	vecBBVertices.push_back(glm::vec3(max_x, min_y, min_z));
	vecBBVertices.push_back(glm::vec3(max_x, max_y, min_z));
	vecBBVertices.push_back(glm::vec3(min_x, max_y, min_z));
	vecBBVertices.push_back(glm::vec3(min_x, min_y, max_z));
	vecBBVertices.push_back(glm::vec3(max_x, min_y, max_z));
	vecBBVertices.push_back(glm::vec3(max_x, max_y, max_z));
	vecBBVertices.push_back(glm::vec3(min_x, max_y, max_z));

	m_pBBoxCube = new BBoxCube(vecBBVertices);
	m_pBBoxCube->Init();
}




