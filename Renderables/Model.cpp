
#include <iostream>
#include "Model.h"
#include "Mesh.h"
#include "../Helpers/VertexStructures.h"
#include "../ShaderEngine/GLSLShader.h"
#include "../MaterialSystem/Material.h"
#include "../Helpers/LogManager.h"

//////////////////////////////////////////////////////////////////////////////////////////
Model::Model(const std::string& path)
{
	LoadModel(path);
}

//////////////////////////////////////////////////////////////////////////////////////////
Model::~Model()
{
	
}

//////////////////////////////////////////////////////////////////////////////////////////
void	Model::LoadModel(const std::string& path)
{
	Assimp::Importer importer;

	const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_CalcTangentSpace | aiProcess_GenSmoothNormals);

	if(!scene || scene->mFlags == AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		std::string err = importer.GetErrorString();
		LogManager::getInstance().WriteToConsole(LOG_ERROR, "Model", err);
		return;
	}

	// retrieve directory path 
	m_Directory = path.substr(0, path.find_last_of('/'));

	// process ASSIMP's root node recursively...
	ProcessNode(scene->mRootNode, scene);
}

//////////////////////////////////////////////////////////////////////////////////////////
void	Model::ProcessNode(aiNode* node, const aiScene* scene)
{
	for (GLuint i = 0; i<node->mNumMeshes; i++)
	{
		// node contains only indices to actual objects in the scene. But, scene contains
		// all the data, node is just to keep things organized.
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		if (!mesh)
		{
			std::string err = "Assimp scene has no valid mesh at current index : " + i;
			LogManager::getInstance().WriteToConsole(LOG_ERROR, "Model", err);
			return;
		}
		else
		{
			m_Meshes.push_back(ProcessMesh(mesh, scene));
		}
	}

	// once we have processed all the meshes, we recursively process each of the children nodes
	for (GLuint j = 0; j<node->mNumChildren; j++)
	{
		ProcessNode(node->mChildren[j], scene);
	}
}

//////////////////////////////////////////////////////////////////////////////////////////
Mesh	Model::ProcessMesh(aiMesh* mesh, const aiScene* scene)
{
	std::vector<VertexPNTBT>	vertices;
	std::vector<GLuint>			indices;
	//std::vector<Texture>		textures;

	// loop through each vertex
	for (GLuint i = 0 ; i<mesh->mNumVertices ; i++)
	{
		VertexPNTBT vertex;
		vertex.position = glm::vec3(mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z);
		vertex.normal = glm::vec3(mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z);
		
		if(mesh->mTangents || mesh->mBitangents)
		{
			vertex.tangent = glm::vec3(mesh->mTangents[i].x, mesh->mTangents[i].y, mesh->mTangents[i].z);
			vertex.binormal = glm::vec3(mesh->mBitangents[i].x, mesh->mBitangents[i].y, mesh->mBitangents[i].z);
		}

		if(mesh->mTextureCoords[0])
		{
			vertex.texcoord = glm::vec2(mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y); 
		}
		else
		{
			vertex.texcoord = glm::vec2(0,0);
		}

		vertices.push_back(vertex);
		
		// Fill up data for bounding box calculations
		m_bbVertices.push_back(vertex.position);
	}

	// loop through each face to fetch indices
	for (GLuint j = 0 ; j<mesh->mNumFaces ; j++)
	{
		aiFace face = mesh->mFaces[j];

		// fetch all the indices
		for (GLuint k = 0 ; k < face.mNumIndices ; k++)
		{
			indices.push_back(face.mIndices[k]);	
		}
	}

	// return  a mesh object created from the extracted mesh data
	return Mesh(vertices, indices);
}

//////////////////////////////////////////////////////////////////////////////////////////
void	Model::Render(GLSLShader* shader, const glm::mat4& world, Material* mat)
{
	for (GLuint i = 0 ; i<m_Meshes.size() ; i++)
	{
		m_Meshes[i].Render(shader, world, mat);
	}
}

std::vector<glm::vec3> Model::GetVertexPositions()
{
	return m_bbVertices;
}