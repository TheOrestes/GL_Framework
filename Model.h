
#pragma once

#include "GL/glew.h"
#include <vector>

struct VertexPNT;
struct Texture;

class Model
{
public:
	Model();
	~Model();

	void					Initialize();
	void					Update(float dt);
	void					Render();
	void					Kill();

	void					AddTexture(const Texture& tex);
	void					AddVertex(const VertexPNT& vert);
	void					AddIndex(const GLuint& index);

private:

	std::vector<VertexPNT>	vertices;
	std::vector<GLuint>		indices;
	std::vector<Texture>	textures;
};
