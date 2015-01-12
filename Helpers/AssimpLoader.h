
#pragma once

#include "GL/glew.h"

#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#include "assimp/postprocess.h"

#include <vector>

struct Model;

class AssimpLoader
{
public:
	static AssimpLoader* getInstance()
	{
		static AssimpLoader loader;
		return &loader;
	}

	~AssimpLoader();
	bool					LoadAssimpMesh(const std::string& fileName);

private:
	AssimpLoader();

	GLuint					shaderID;
	std::vector<Model>		models;
};