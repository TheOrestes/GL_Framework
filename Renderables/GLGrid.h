
#pragma once

#include "glew.h"
#include "..\Helpers\VertexStructures.h"

class GLGrid 
{
public:
	GLGrid();
	~GLGrid();

	void		Init();
	void		Update(float dt);
	void		Render();
	void		Kill();

	void		SetPosition(const glm::vec3& loc);

private:
	GLGrid(const GLGrid&);
	void operator=(const GLGrid&);

	void		SetupViewProjMatrix();

	GLuint		vbo;				// vertex buffer object
	GLuint		ibo;				// index buffer object
	GLuint		shader;				// shader object

	GLuint		vao;				// vertex array object

	GLint		posAttrib;			// vertex attribute id
	GLint		colAttrib;			// color attribute id
	VertexPC	vertices[8];		// vertices of a cube
	GLuint		indices[36];		// indices of a cube

	GLint		hWorld;
	GLint		hView;
	GLint		hProj;

	bool		m_bWireframe;

	glm::mat4   matWorld;
	glm::mat4	matView;
	glm::mat4	matProj;
};