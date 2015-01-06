
#pragma once

#include <string>
#include <vector>
#include "GLMesh.h"
#include "../Helpers/VertexStructures.h"

#include "assimp/Importer.hpp"
#include "assimp/postprocess.h"
#include "assimp/scene.h"

class Model
{
public:
	Model();
	~Model();
	

private:
	std::vector<GLMesh>		m_vecMeshes;
	std::string				m_strDirectory;

	void					LoadModel(const std::string& path);
	void					ProcessNode(aiNode* node, const aiScene* scene);
	void					ProcessMesh(aiNode* node, const aiScene* scene);

	std::vector<Texture>	LoadMaterialTextures(aiMaterial* material, aiTextureType type, const std::string& typeName);
};