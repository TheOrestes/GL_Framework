
#pragma once

#include "glew.h"
#include "..\Helpers\VertexStructures.h"

class GLSLShader;

class GLCubeNormal
{
public:
	GLCubeNormal();
	GLCubeNormal(const glm::vec4& color);

	~GLCubeNormal();

	void		Init();
	void		Update(float dt);
	void		Render();
	void		Kill();

	void		SetPosition(const glm::vec3& loc);
	void		SetScale(const glm::vec3& sc);
	void		SetRotation(const glm::vec3& ax, float angle);

private:
	GLCubeNormal(const GLCubeNormal&);
	void operator=(const GLCubeNormal&);

	void		SetupViewProjMatrix();

	GLuint		vbo;				// vertex buffer object
	GLuint		ibo;				// index buffer object

	GLuint		vao;				// vertex array object

	GLint		posAttrib;			// vertex attribute id
	GLint		colAttrib;			// color attribute id
	VertexPN	vertices[8];		// vertices of a cube
	GLuint		indices[36];		// indices of a cube

	GLint		hWorld;				// handle to World matrix
	GLint		hView;				// handle to View matrix
	GLint		hProj;				// handle to projection matrix
	GLint		hLightDir;			// handle to global light direction
	GLint		hObjectColor;		// handle for an object color

	glm::vec3	vecPosition;		// World Space Position
	glm::vec3	vecScale;			// Scale of an object
	glm::vec3	vecRotationAxis;	// rotation axis
	float		m_fAngle;			// angle of rotation

	glm::vec4	m_color;

	bool		m_bWireframe;		// wire-frame rendering flag

	GLSLShader*	m_pShader;			// Ptr to shader object

	glm::mat4   matWorld;
	glm::mat4	matView;
	glm::mat4	matProj;
};