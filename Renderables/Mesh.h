
#pragma once

#include "GL/glew.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_inverse.hpp"
#include <vector>

struct VertexPNT;
struct Texture;
class GLSLShader;

class Mesh
{
public:
	Mesh(std::vector<VertexPNT> vertices, std::vector<GLuint> indices, std::vector<Texture> textures);
	~Mesh();

	std::vector<VertexPNT>	m_vertices;
	std::vector<GLuint>		m_indices;
	std::vector<Texture>	m_textures;

	void		Render(GLSLShader* shader, const glm::mat4& world);

private:
	GLuint		vao;
	GLuint		vbo;
	GLuint		ibo;

	void		SetupMesh();
};