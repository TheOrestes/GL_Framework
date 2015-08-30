
#pragma once

#include "GL/glew.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_inverse.hpp"
#include <vector>

struct VertexPNTBT;
struct Texture;
class GLSLShader;
struct Material;

class Mesh
{
public:
	Mesh(std::vector<VertexPNTBT> vertices, std::vector<GLuint> indices, std::vector<Texture> textures);
	~Mesh();

	std::vector<VertexPNTBT>	m_vertices;
	std::vector<GLuint>		m_indices;
	std::vector<Texture>	m_textures;

	void		Kill();

	void		SetMaterialProperties(int shaderID, Material* mat);
	void		PointLightIlluminance(int shaderID);
	void		DirectionalLightIlluminance(int shaderID);

	void		SetShaderVariables(int shaderID, const glm::mat4& world, Material* mat);
	void		Render(GLSLShader* shader, const glm::mat4& world, Material* mat);

private:
	GLuint		vao;
	GLuint		vbo;
	GLuint		ibo;

	void		SetupMesh();
};