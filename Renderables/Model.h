
#pragma once

#include <string>
#include <vector>
#include "../Helpers/VertexStructures.h"

#include "assimp/Importer.hpp"
#include "assimp/postprocess.h"
#include "assimp/scene.h"

class Mesh;
class GLSLShader;
struct Material;

class Model
{
public:
	Model(const std::string& path);
	~Model();
	
	void					Render(GLSLShader* shader, const glm::mat4& world, Material* mat);

private:
	std::vector<Mesh>		m_Meshes;
	std::string				m_Directory;
	std::vector<Texture>	m_textures_loaded;

	void					LoadModel(const std::string& path);
	void					ProcessNode(aiNode* node, const aiScene* scene);
	Mesh					ProcessMesh(aiMesh* mesh, const aiScene* scene);

	std::vector<Texture>	LoadMaterialTextures(aiMaterial* material, aiTextureType type, const std::string& typeName);
};