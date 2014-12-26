
#pragma once

#include "GL\glew.h"
#include "..\Helpers\VertexStructures.h"

class GLCube
{
public:
	GLCube();
	GLCube(const glm::vec4& color);

	~GLCube();

	void		Init();
	void		Update(float dt);
	void		Render();
	void		Kill();

	void		SetPosition(const glm::vec3& loc);

private:
	GLCube(const GLCube&);
	void operator=(const GLCube&);

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

	glm::vec3	vecPosition;		// World Space Position

	glm::mat4   matWorld;
	glm::mat4	matView;
	glm::mat4	matProj;
};