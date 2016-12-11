
#include "GLSkybox.h"
#include "../ShaderEngine/GLSLShader.h"
#include "../Camera/Camera.h"
#include "../Helpers/TextureManager.h"
#include "../Helpers/LogManager.h"

#include "assimp/Importer.hpp"
#include "assimp/postprocess.h"
#include "assimp/scene.h"

//////////////////////////////////////////////////////////////////////////////////////////
GLSkybox::GLSkybox()
{
	m_vertices.clear();
	m_indices.clear();

	// set position to origin
	vecPosition = glm::vec3(0,0,0);
}

//////////////////////////////////////////////////////////////////////////////////////////
GLSkybox::~GLSkybox()
{
	Kill();
}

//////////////////////////////////////////////////////////////////////////////////////////
void GLSkybox::LoadModel()
{
	Assimp::Importer importer;

	const aiScene* scene = importer.ReadFile("Data/Models/SkySphere/SkySphere.fbx", aiProcess_Triangulate | aiProcess_GenSmoothNormals);

	if (!scene || scene->mFlags == AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		std::string err = importer.GetErrorString();
		LogManager::getInstance().WriteToConsole(LOG_ERROR, "Model", err);
		return;
	}

	// Since we are sure that we just have one mesh of sphere, we can safely take 0th index!
	aiMesh* mesh = scene->mMeshes[0];
	
	// loop through each vertex
	for (GLuint i = 0; i < mesh->mNumVertices; i++)
	{
		VertexPNT vertex;
		vertex.position = glm::vec3(mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z);
		vertex.normal = glm::vec3(mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z);
		vertex.texcoord = glm::vec2(mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y);

		m_vertices.push_back(vertex);
	}

	// loop through each face to fetch indices
	for (GLuint j = 0; j < mesh->mNumFaces; j++)
	{
		aiFace face = mesh->mFaces[j];

		// fetch all the indices
		for (GLuint k = 0; k < face.mNumIndices; k++)
		{
			m_indices.push_back(face.mIndices[k]);
		}
	}
}

//////////////////////////////////////////////////////////////////////////////////////////
void GLSkybox::Init()
{
	LoadModel();

	// create skybox instance...
	m_pShader = new GLSLShader("Shaders/vsSkybox.glsl", "Shaders/psSkybox.glsl");

	// Load HDRI & assign id ...
	tbo = TextureManager::getInstannce().LoadTextureFromFile("Data/HDRI/uffizi-large.hdr");

	// create vao
	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);
	glGenBuffers(1, &ibo);

	glBindVertexArray(vao);

		// create vbo
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(GL_ARRAY_BUFFER, m_vertices.size() * sizeof(VertexPNT), &m_vertices[0], GL_STATIC_DRAW);

		// create ibo
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_indices.size() * sizeof(GLuint), &m_indices[0], GL_STATIC_DRAW);

		GLuint shader = m_pShader->GetShaderID();

		// position
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(VertexPNT), (void*)0);

		// normal 
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(VertexPNT), (void*)(offsetof(VertexPNT, normal)));

		// texcoord
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(VertexPNT), (void*)(offsetof(VertexPNT, texcoord)));

	glBindVertexArray(0);
}

//////////////////////////////////////////////////////////////////////////////////////////
void GLSkybox::Render()
{
	// So based on shader for skybox, we have made sure that third component equal to w, 
	// perspective divide always returns 1. This way depth value is always 1 which is at
    // back of all the geometries rendered. 
	// The depth buffer will be filled with values of 1.0 for the skybox, so we need to 
	// make sure the skybox passes the depth tests with values less than or equal to the 
	// depth buffer instead of less than. 
	glDepthFunc(GL_LEQUAL);
	m_pShader->Use();

	GLuint shader = m_pShader->GetShaderID();

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, tbo);

	glUniformMatrix4fv(glGetUniformLocation(shader, "matWorld"), 1, GL_FALSE, glm::value_ptr(matWorld));
	glUniformMatrix4fv(glGetUniformLocation(shader, "matView"), 1, GL_FALSE, glm::value_ptr(matView));
	glUniformMatrix4fv(glGetUniformLocation(shader, "matProj"), 1, GL_FALSE, glm::value_ptr(matProj));

	glBindVertexArray(vao);
	glDrawElements(GL_TRIANGLES, m_indices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);

	// Set depth function back to default
	glDepthFunc(GL_LESS);

	glBindTexture(GL_TEXTURE_2D, 0);
}

//////////////////////////////////////////////////////////////////////////////////////////
void GLSkybox::Update( float dt )
{
	static float angle2 = 0;
	angle2 += dt;

	glm::mat4 model;
	model = glm::rotate(model, angle2, glm::vec3(0,1,0));

	matWorld = glm::translate(glm::mat4(1), vecPosition);
	matView = Camera::getInstance().getViewMatrix();
	matProj = Camera::getInstance().getProjectionMatrix();
}

//////////////////////////////////////////////////////////////////////////////////////////
void GLSkybox::Kill()
{
	m_vertices.clear();
	m_indices.clear();

	delete m_pShader;

	glDeleteBuffers(1, &vbo);
	glDeleteBuffers(1, &ibo);
	glDeleteBuffers(1, &tbo);
	glDeleteVertexArrays(1, &vao);
}



