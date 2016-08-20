
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
//std::vector<Texture>	Model::LoadMaterialTextures(aiMaterial* material, aiTextureType type, const std::string& typeName)
//{
//	std::vector<Texture> textures;
//
//	for (GLuint i = 0 ; i<material->GetTextureCount(type) ; i++)
//	{
//		aiString str;
//		material->GetTexture(type, i, &str);
//
//		// check if texture was loaded earlier, if yes then continue to next iteration
//		bool skip = false;
//		for (GLuint j = 0 ; j<m_textures_loaded.size() ; j++)
//		{
//			if (m_textures_loaded[i].path == str)
//			{
//				textures.push_back(m_textures_loaded[j]);
//				skip = true;	// texture with the filename has already been loaded, continue to next one.
//				break;
//			}
//		}
//
//		if(!skip)
//		{
//			// Create full path for textures
//			std::string fullTexturePath = m_Directory + '/' + str.C_Str();
//			// if texture is not loaded, load it now.
//			Texture texture;
//			texture.id = TextureManager::getInstannce().LoadTextureFromFile(fullTexturePath); //TextureFromFile(str.C_Str(), m_Directory);
//			texture.name = typeName;
//			texture.path = str;
//
//			textures.push_back(texture);
//			m_textures_loaded.push_back(texture);
//		}
//	}
//
//	// If there is no texture on the model, load the default chequered texture!
//	// second if condition is added so that default texture is loaded only for the diffuse part & not specular!
//	/*if( material->GetTextureCount(aiTextureType_DIFFUSE) == 0 && type == aiTextureType_DIFFUSE)
//	{
//		Texture texture;
//		texture.id = TextureManager::getInstannce().Load2DTextureFromFile("UV_mapper.jpg", m_Directory); //TextureFromFile("UV_mapper.jpg", m_Directory);
//		texture.name = typeName;
//		texture.path = "UV_mapper.jpg";
//
//		textures.push_back(texture);
//		m_textures_loaded.push_back(texture);
//	}*/
//
//	return textures;
//}

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

	// process materials
	/*if (mesh->mMaterialIndex >= 0)
	{
		aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

		unsigned int nDiff = material->GetTextureCount(aiTextureType_DIFFUSE);
		unsigned int nHeight = material->GetTextureCount(aiTextureType_HEIGHT);
		unsigned int nNormal = material->GetTextureCount(aiTextureType_NORMALS);
		unsigned int nEmissive = material->GetTextureCount(aiTextureType_EMISSIVE);
		unsigned int nSpecular = material->GetTextureCount(aiTextureType_SPECULAR);
		unsigned int nAmbient = material->GetTextureCount(aiTextureType_AMBIENT);
		unsigned int nShininess = material->GetTextureCount(aiTextureType_SHININESS);
		unsigned int nDisplacement = material->GetTextureCount(aiTextureType_DISPLACEMENT);
		unsigned int nLightmap = material->GetTextureCount(aiTextureType_LIGHTMAP);
		unsigned int nReflection = material->GetTextureCount(aiTextureType_REFLECTION);
		unsigned int nUnknown = material->GetTextureCount(aiTextureType_UNKNOWN);
		
		
		// diffuse maps
		std::vector<Texture> diffuseMaps = LoadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
		textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());

		// height maps
		std::vector<Texture> heightMaps = LoadMaterialTextures(material, aiTextureType_HEIGHT, "texture_height");
		textures.insert(textures.end(), heightMaps.begin(), heightMaps.end());

		// normal maps
		std::vector<Texture> normalMaps = LoadMaterialTextures(material, aiTextureType_NORMALS, "texture_normal");
		textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());

		// emissive maps
		std::vector<Texture> emissiveMaps = LoadMaterialTextures(material, aiTextureType_EMISSIVE, "texture_emissive");
		textures.insert(textures.end(), emissiveMaps.begin(), emissiveMaps.end());

		// specular maps
		std::vector<Texture> specularMaps = LoadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
		textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
		
		// ambient maps
		std::vector<Texture> ambientMaps = LoadMaterialTextures(material, aiTextureType_AMBIENT, "texture_ambient");
		textures.insert(textures.end(), ambientMaps.begin(), ambientMaps.end());

		// shininess maps
		std::vector<Texture> shininessMaps = LoadMaterialTextures(material, aiTextureType_SHININESS, "texture_shininess");
		textures.insert(textures.end(), shininessMaps.begin(), shininessMaps.end());

		// displacement maps
		std::vector<Texture> displacementMaps = LoadMaterialTextures(material, aiTextureType_DISPLACEMENT, "texture_displacement");
		textures.insert(textures.end(), displacementMaps.begin(), displacementMaps.end());

		// lightmap maps
		std::vector<Texture> lightMaps = LoadMaterialTextures(material, aiTextureType_LIGHTMAP, "texture_lightmap");
		textures.insert(textures.end(), lightMaps.begin(), lightMaps.end());

		// reflection maps
		std::vector<Texture> reflectionMaps = LoadMaterialTextures(material, aiTextureType_REFLECTION, "texture_reflection");
		textures.insert(textures.end(), reflectionMaps.begin(), reflectionMaps.end());

		// unknown maps
		std::vector<Texture> unknownMaps = LoadMaterialTextures(material, aiTextureType_UNKNOWN, "texture_unknown");
		textures.insert(textures.end(), unknownMaps.begin(), unknownMaps.end());

	}*/

	// return  a mesh object created from the extracted mesh data
	return Mesh(vertices, indices);
}

//////////////////////////////////////////////////////////////////////////////////////////
void	Model::ProcessNode(aiNode* node, const aiScene* scene)
{
	for (GLuint i = 0 ; i<node->mNumMeshes ; i++)
	{
		// node contains only indices to actual objects in the scene. But, scene contains
		// all the data, node is just to keep things organized.
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		if(!mesh)
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
	for (GLuint j = 0 ; j<node->mNumChildren ; j++)
	{
		ProcessNode(node->mChildren[j], scene);
	}
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