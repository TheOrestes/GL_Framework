
#include "StaticObject.h"
#include "../ShaderEngine/GLSLShader.h"
#include "../Renderables/Model.h"
#include "../MaterialSystem/Material.h"
#include "../Renderables/BBoxCube.h"
#include "../UI/UIManager.h"
#include "../Helpers/LogManager.h"
#include "../Helpers/Helper.h"

#include "nfd.h"

//////////////////////////////////////////////////////////////////////////////////////////
StaticObject::StaticObject()
{
	m_pObjData = nullptr;
	m_pShader = nullptr;
	m_pModel = nullptr;
	m_pMaterial = nullptr;
	m_pBBoxCube = nullptr;
}

//////////////////////////////////////////////////////////////////////////////////////////
StaticObject::StaticObject(const StaticObjectData& data)
	:	m_pShader(nullptr),
		m_pModel(nullptr),
		m_pBBoxCube(nullptr)
{
	m_pObjData = new StaticObjectData(data);
	m_vecPosition  = data.position;
	m_fAngle = data.angle;
	m_vecRotation = data.rotation;
	m_fScale = data.scale; 
}

//////////////////////////////////////////////////////////////////////////////////////////
StaticObject::~StaticObject()
{
	Kill();
}

//////////////////////////////////////////////////////////////////////////////////////////
void StaticObject::Init()
{
	std::string msg;
	std::string vertShaderPath;
	vertShaderPath = "Shaders/vs" + m_pObjData->shader + ".glsl";

	std::string fragShaderPath;
	fragShaderPath = "Shaders/ps" + m_pObjData->shader + ".glsl";

	m_pShader = new GLSLShader(vertShaderPath, fragShaderPath);
	if(m_pShader) 
	{
		msg = m_pObjData->shader + " Compiled & Loaded...";
		LogManager::getInstance().WriteToConsole(LOG_INFO, "StaticObject", msg);
	}

	// initialize model
	m_pModel = new Model(m_pObjData->path);
	if(m_pModel)
	{
		msg = m_pObjData->path + " Loaded...";
		LogManager::getInstance().WriteToConsole(LOG_INFO, "StaticObject", msg);
	}

	// initialize material
	m_pMaterial = new Material();
		
	// Initial transformation...
	glm::mat4 model;
	model = glm::translate(model, m_vecPosition); 
	model = glm::rotate(model, m_fAngle, m_vecRotation);

	// Support & Apply uniform scale
	glm::vec3 m_vecScale = glm::vec3(m_fScale);
	model = glm::scale(model, m_vecScale);	

	m_matWorld = model;

	// initialize bounding box
	InitBBox(m_pModel->GetVertexPositions());

	// Initialize AntTweakBar UI
	InitUI();
}

//////////////////////////////////////////////////////////////////////////////////////////
void TW_CALL UpdateTexture(void* data)
{
	nfdchar_t* outPath = nullptr;
	nfdresult_t result = NFD_OpenDialog("png,jpg,tif,bmp,tga,hdr", nullptr, &outPath);
	if (result == NFD_OKAY)
	{
		TextureProperty* prop = static_cast<TextureProperty*>(data);
		prop->setPath(outPath);
		prop->changed = true;
	}
}

//////////////////////////////////////////////////////////////////////////////////////////
void TW_CALL UpdateMesh(void* data)
{
	nfdchar_t* outPath = nullptr;
	nfdresult_t result = NFD_OpenDialog("fbx,obj,md5,md3,md2,collada", nullptr, &outPath);

	if (result == NFD_OKAY)
	{
		StaticObjectData* objData = static_cast<StaticObjectData*>(data);
		objData->path = (std::string)outPath;
		objData->changed = true;
	}
}

//////////////////////////////////////////////////////////////////////////////////////////
void StaticObject::InitUI()
{
	m_pUIBar = TwNewBar(m_pObjData->name.c_str());

	// Create a new TwType to edit 3D points: a struct that contains three floats
	struct Point
	{
		float x, y, z;
	};

	TwStructMember pointMembers[] = 
	{
		{ "X", TW_TYPE_FLOAT, offsetof(Point, x), " Min=0.0 Max=1000 Step=0.1 " },
		{ "Y", TW_TYPE_FLOAT, offsetof(Point, y), " Min=0.0 Max=1000 Step=0.1 " },
		{ "Z", TW_TYPE_FLOAT, offsetof(Point, z), " Min=0.0 Max=1000 Step=0.1 " } 
	};
	TwType pointType = TwDefineStruct("Position", pointMembers, 3, sizeof(Point), NULL, NULL);

	TwAddButton(m_pUIBar, "RenderMesh", UpdateMesh, m_pObjData, "label='Render Mesh'");
	TwAddVarRW(m_pUIBar, "Translation", pointType, glm::value_ptr(m_vecPosition), "label='Translation'");
	TwAddVarRW(m_pUIBar, "Rotation", pointType, glm::value_ptr(m_vecRotation), "label='Rotation'");
	TwAddVarRW(m_pUIBar, "Scale", TW_TYPE_FLOAT, &m_fScale, "label='Scale' min=0.1 max=1000 step=0.01");
	TwAddVarRW(m_pUIBar, "Show Bounds", TW_TYPE_BOOLCPP, &(m_pObjData->showBBox), "label='Show Bounds'");

	TwAddVarRW(m_pUIBar, "AlbdeoColor", TW_TYPE_COLOR4F, glm::value_ptr(m_pMaterial->m_colAlbedo), "label='Albdeo Color'");
	TwAddVarRW(m_pUIBar, "EmissiveColor", TW_TYPE_COLOR4F, glm::value_ptr(m_pMaterial->m_colEmissive), "label='Emissive Color'");
	TwAddVarRW(m_pUIBar, "Roughness", TW_TYPE_FLOAT, &(m_pMaterial->m_fRoughness), 
						 "label='Roughness' min=0 max=1 step=0.1");
	TwAddVarRW(m_pUIBar, "Metallic", TW_TYPE_FLOAT, &(m_pMaterial->m_fMetallic), 
						 "label='Metallic' min=0 max=1 step=0.1");

	TwAddButton(m_pUIBar, "AlbedoTexture", UpdateTexture, &(m_pMaterial->m_pTexAlbedo), "label='Albedo Texture'");
	TwAddButton(m_pUIBar, "EmissionTexture", UpdateTexture, &(m_pMaterial->m_pTexEmission), "label='Emission Texture'");
	TwAddButton(m_pUIBar, "HeightTexture", UpdateTexture, &(m_pMaterial->m_pTexHeight), "label='Height Texture'");
	TwAddButton(m_pUIBar, "NormalTexture", UpdateTexture, &(m_pMaterial->m_pTexNormal), "label='Normal Texture'");
	TwAddButton(m_pUIBar, "OcclusionTexture", UpdateTexture, &(m_pMaterial->m_pTexOcclusion), "label='Occlusion Texture'");
	TwAddButton(m_pUIBar, "SpecularTexture", UpdateTexture, &(m_pMaterial->m_pTexSpecular), "label='Specular Texture'");
}

//////////////////////////////////////////////////////////////////////////////////////////
void StaticObject::Kill()
{
	delete m_pObjData;
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

	glm::vec3 m_vecScale = glm::vec3(m_fScale);
	model = glm::scale(model, m_vecScale);	

	m_matWorld = model;

	m_pBBoxCube->UpdateBB(m_matWorld, dt);
}

//////////////////////////////////////////////////////////////////////////////////////////
void StaticObject::Render()
{
	if (m_pObjData && m_pObjData->changed)
	{
		delete m_pModel;
		m_pModel = nullptr;

		m_pModel = new Model(m_pObjData->path);
		if (m_pModel)
		{
			LogManager::getInstance().WriteToConsole(LOG_INFO, "StaticObject", m_pObjData->path + " Loaded...");
			m_pObjData->changed = false;
		}
	}

	m_pModel->Render(m_pShader, m_matWorld, m_pMaterial);
	
	if (m_pObjData->showBBox)
	{
		m_pBBoxCube->Render(m_matWorld);
	}

	//TwDraw();
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
void StaticObject::SetScale( const float& sc )
{
	m_fScale = sc;
}

//////////////////////////////////////////////////////////////////////////////////////////
void StaticObject::SetCentroid( const glm::vec3& center )
{
	m_vecCentroid = center;
}

//////////////////////////////////////////////////////////////////////////////////////////
void StaticObject::ShowBBox( bool flag )
{
	m_pObjData->showBBox = flag;
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




