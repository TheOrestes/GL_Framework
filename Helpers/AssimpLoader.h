
#pragma once

#include "assimp/Importer.hpp"
#include "assimp/postprocess.h"
#include "assimp/scene.h"

#include <vector>

#include "../Renderables/GLMesh.h"

class AssimpLoader
{
public:
	static AssimpLoader& getInstance()
	{
		static AssimpLoader loader;
		return loader;
	}

	~AssimpLoader();

	bool				LoadAssimpMesh(const std::string& path, GLMesh** mesh);

private:
	AssimpLoader();

	// Data structures to hold values for our GLMesh
	std::vector<VertexPN>	m_vecVertices;
	std::vector<GLuint>		m_vecIndices;
	//std::vector<Texture>	m_vecTextures;
};

