
#pragma once

#include "glew.h"
#include "..\Helpers\VertexStructures.h"

class GLSLShader;

class BBoxCube
{
public:
	BBoxCube();
	BBoxCube(std::vector<glm::vec3> _positions);

	~BBoxCube();

	void		Init();
	void		UpdateBB( const glm::mat4& _matWorld, float dt);
	void		Render(const glm::mat4& _matWorld );
	void		Kill();

	void		SetPosition(const glm::vec3& loc);
	void		SetScale(const glm::vec3& sc);
	void		SetRotation(const glm::vec3& ax, float angle);

private:
	BBoxCube(const BBoxCube&);
	void operator=(const BBoxCube&);

	void		SetupViewProjMatrix();

	GLuint		vbo;				// vertex buffer object
	GLuint		ibo;				// index buffer object

	GLuint		vao;				// vertex array object

	GLint		posAttrib;			// vertex attribute id
	GLint		colAttrib;			// color attribute id
	VertexPC	vertices[24];		// vertices of a cube
	GLuint		indices[36];		// indices of a cube

	GLint		hWorld;				// handle to World matrix
	GLint		hView;				// handle to View matrix
	GLint		hProj;				// handle to projection matrix

	glm::vec3	vecPosition;		// World Space Position
	glm::vec3	vecScale;			// Scale of an object
	glm::vec3	vecRotationAxis;	// rotation axis
	float		m_fAngle;			// angle of rotation

	bool		m_bWireframe;		// wire-frame rendering flag

	GLSLShader*	m_pShader;			// Ptr to shader object

	glm::mat4   matWorld;
	glm::mat4	matView;
	glm::mat4	matProj;
};