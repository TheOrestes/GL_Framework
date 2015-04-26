
#include <iostream>
#include "Model.h"
#include "Mesh.h"
#include "../Helpers/VertexStructures.h"
#include "../ShaderEngine/GLSLShader.h"
#include "../Helpers/TextureManager.h"

//////////////////////////////////////////////////////////////////////////////////////////
Model::Model(const std::string& path)
{
	LoadModel(path);
}

//////////////////////////////////////////////////////////////////////////////////////////
Model::~Model()
{
	FreeImage_DeInitialise();
}

//////////////////////////////////////////////////////////////////////////////////////////
void	Model::LoadModel(const std::string& path)
{
	Assimp::Importer importer;

	const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate);

	if(!scene || scene->mFlags == AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		std::cout << "ASSIMP::ERROR = " << importer.GetErrorString() << std::endl; 
		return;
	}

	// retrieve directory path 
	m_Directory = path.substr(0, path.find_last_of('/'));

	// process ASSIMP's root node recursively...
	ProcessNode(scene->mRootNode, scene);
}


//////////////////////////////////////////////////////////////////////////////////////////
std::vector<Texture>	Model::LoadMaterialTextures(aiMaterial* material, aiTextureType type, const std::string& typeName)
{
	std::vector<Texture> textures;

	for (GLuint i = 0 ; i<material->GetTextureCount(type) ; i++)
	{
		aiString str;
		material->GetTexture(type, i, &str);

		// check if texture was loaded earlier, if yes then continue to next iteration
		bool skip = false;
		for (GLuint j = 0 ; j<m_textures_loaded.size() ; j++)
		{
			if (m_textures_loaded[i].path == str)
			{
				textures.push_back(m_textures_loaded[j]);
				skip = true;	// texture with the filename has already been loaded, continue to next one.
				break;
			}
		}

		if(!skip)
		{
			// if texture is not loaded, load it now.
			Texture texture;
			texture.id = TextureManager::getInstannce().Load2DTextureFromFile(str.C_Str(), m_Directory); //TextureFromFile(str.C_Str(), m_Directory);
			texture.name = typeName;
			texture.path = str;

			textures.push_back(texture);
			m_textures_loaded.push_back(texture);
		}
	}

	// If there is no texture on the model, load the default chequered texture!
	// second if condition is added so that default texture is loaded only for the diffuse part & not specular!
	if( material->GetTextureCount(aiTextureType_DIFFUSE) == 0 && type == aiTextureType_DIFFUSE)
	{
		Texture texture;
		texture.id = TextureManager::getInstannce().Load2DTextureFromFile("UV_mapper.jpg", m_Directory); //TextureFromFile("UV_mapper.jpg", m_Directory);
		texture.name = typeName;
		texture.path = "UV_mapper.jpg";

		textures.push_back(texture);
		m_textures_loaded.push_back(texture);
	}

	return textures;
}

//////////////////////////////////////////////////////////////////////////////////////////
Mesh	Model::ProcessMesh(aiMesh* mesh, const aiScene* scene)
{
	std::vector<VertexPNT>	vertices;
	std::vector<GLuint>		indices;
	std::vector<Texture>	textures;

	// loop through each vertex
	for (GLuint i = 0 ; i<mesh->mNumVertices ; i++)
	{
		VertexPNT vertex;
		vertex.position = glm::vec3(mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z);
		vertex.normal = glm::vec3(mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z);

		if(mesh->mTextureCoords[0])
		{
			vertex.texcoord = glm::vec2(mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y); 
		}
		else
		{
			vertex.texcoord = glm::vec2(0,0);
		}

		vertices.push_back(vertex);
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

	// process materials
	if (mesh->mMaterialIndex >= 0)
	{
		aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
		
		// diffuse maps
		std::vector<Texture> diffuseMaps = LoadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
		textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());

		// specular maps
		std::vector<Texture> specularMaps = LoadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
		textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
	}

	// return  a mesh object created from the extracted mesh data
	return Mesh(vertices, indices, textures);
}

//////////////////////////////////////////////////////////////////////////////////////////
void	Model::ProcessNode(aiNode* node, const aiScene* scene)
{
	for (GLuint i = 0 ; i<node->mNumMeshes ; i++)
	{
		// node contains only indices to actual objects in the scene. But, scene contains
		// all the data, node is just to keep things organized.
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		m_Meshes.push_back(ProcessMesh(mesh, scene));
	}

	// once we have processed all the meshes, we recursively process each of the children nodes
	for (GLuint j = 0 ; j<node->mNumChildren ; j++)
	{
		ProcessNode(node->mChildren[j], scene);
	}
}

//////////////////////////////////////////////////////////////////////////////////////////
void	Model::Render(GLSLShader* shader, const glm::mat4& world)
{
	for (GLuint i = 0 ; i<m_Meshes.size() ; i++)
	{
		m_Meshes[i].Render(shader, world);
	}
}