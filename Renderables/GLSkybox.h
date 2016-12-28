
#pragma once

#include <vector>
#include "../Helpers/VertexStructures.h"


class GLSLShader;
struct VertexPNT;

class GLSkybox
{
public:
	static GLSkybox& getInstance()
	{
		static GLSkybox instance;
		return instance;
	}

	~GLSkybox();

	void		Init();
	void		Render();
	void		Update(float dt);
	void		Kill();

	GLuint		GetEnvironmentMapID() { return tbo; }
	void		SetHDRIPath(const std::string& str) { m_bChanged = true; m_strPathHDRI = str; }

private:
	GLSkybox();
	void		LoadModel();

	GLuint		vao;
	GLuint		vbo;
	GLuint		ibo;
	GLuint		tbo;

	GLint		posAttrib;

	std::vector<VertexPNT>	m_vertices;
	std::vector<GLuint>		m_indices;

	GLint		hWorld;
	GLint		hView;
	GLint		hProj;

	glm::vec3	vecPosition;

	GLSLShader*	m_pShader;

	glm::mat4	matWorld;
	glm::mat4	matView;
	glm::mat4   matProj;

	std::string	m_strPathHDRI;
	bool		m_bChanged;
};