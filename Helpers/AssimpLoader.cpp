
#include <iostream>
#include "AssimpLoader.h"
#include "Helper.h"
#include "VertexStructures.h"
#include "Texture.h"
#include "../ShaderEngine/GLSLParser.h"


//////////////////////////////////////////////////////////////////////////////////////////
AssimpLoader::AssimpLoader()
{
	models.clear();
	shaderID = -1;
}

//////////////////////////////////////////////////////////////////////////////////////////
AssimpLoader::~AssimpLoader()
{

}

//////////////////////////////////////////////////////////////////////////////////////////
bool	AssimpLoader::LoadAssimpMesh(const std::string& fileName)
{
	// Create a shader
	shaderID = GLSLParser::getInstance().LoadShader("Data/vsLighting.glsl", "Data/psLighting.glsl");

	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(fileName, aiProcess_Triangulate | aiProcess_FlipUVs);

	if (!scene)
	{
		std::cout << "ASSIMP::ERROR = " << importer.GetErrorString() << std::endl;
		return false;
	}

	// if assimp scene has mesh information
	if (scene->HasMeshes())
	{
		for (unsigned int i = 0 ; i<scene->mNumMeshes ; i++)
		{
			// create temporary model object as a container...
			Model* tmpModel;

			aiMesh* mesh = scene->mMeshes[i];
			if (!mesh)
			{
				std::cout << "ASSIMP::ERROR = " << i << " index mesh problem!" << std::endl;
				return false;
			}

			// Extract texture information per mesh...
			aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

			// Create diffuse texture
			if (material && material->GetTextureCount(aiTextureType_DIFFUSE) > 0)
			{
				aiString diffuseTextureName;
				material->GetTexture(aiTextureType_DIFFUSE, 0, &diffuseTextureName);

				// generate absolute path before texture loading...
				std::string directoryPath = Helper::ExtractDirectory(fileName);
				std::string absolutePath = directoryPath.append(diffuseTextureName.data);

				// create temporary texture...
				Texture tex(GL_TEXTURE_2D, absolutePath);
				if(!tex.Load())
				{
					std::cout << "ASSIMP::ERROR = " << diffuseTextureName.C_Str() << " loading FAILED!" << std::endl;
				}

				// push this texture object into the list of texture objects...
				tmpModel->AddTexture(tex);

			}

			// Extract Vertex information per mesh...
			for (unsigned int j = 0 ; j < mesh->mNumVertices ; j++)
			{
				VertexPNT vertex;

				vertex.position =	glm::vec3(mesh->mVertices[j].x, mesh->mVertices[j].y, mesh->mVertices[j].z);
				vertex.normal	=	glm::vec3(mesh->mNormals[j].x, mesh->mNormals[j].y, mesh->mNormals[j].z);

				if(mesh->HasTextureCoords(0))
				{
					vertex.texcoord = glm::vec2(mesh->mTextureCoords[0][j].x, mesh->mTextureCoords[0][j].y);
				}
				else
				{
					vertex.texcoord = glm::vec2(0,0);
				}

				// push this vertex into the list of vertices...
				tmpModel->AddVertex(vertex);
			}
		}
	}
}