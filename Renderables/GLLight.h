
#pragma once

#include "..\Helpers\VertexStructures.h"

class GLSLShader;

class GLLight
{
public:
	GLLight();
	GLLight(const glm::vec3& color);

	virtual ~GLLight();

	virtual void		Init();
	virtual void		Update(float dt);
	virtual void		Render();
	virtual void		Kill();

	inline  void		SetPosition(const glm::vec3& pos){ position = pos; }
	inline  glm::vec3	GetPosition(){ return position; }
	
private:
	GLLight(const GLLight&);
	void operator=(const GLLight&);

	void		SetupViewProjMatrix();

	GLuint		vbo;				// vertex buffer object
	GLuint		ibo;				// index buffer object

	GLuint		vao;				// vertex array object

	GLint		posAttrib;			// vertex attribute id
	GLint		colAttrib;			// color attribute id
	VertexPC	vertices[8];		// vertices of a cube
	GLuint		indices[36];		// indices of a cube

	glm::vec3	position;			// light mesh position

	GLint		hWorld;				// handle to World matrix
	GLint		hView;				// handle to View matrix
	GLint		hProj;				// handle to projection matrix

	GLSLShader*	m_pShader;			// Ptr to shader object

	glm::mat4   matWorld;
	glm::mat4	matView;
	glm::mat4	matProj;
};