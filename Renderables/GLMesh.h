
#pragma once

#include <vector>

#include "GL\glew.h"
#include "..\Helpers\VertexStructures.h"

class GLMesh
{
public:
	GLMesh();
	GLMesh(const std::vector<VertexPN>& vert, const std::vector<GLuint>& ind/*, const std::vector<Texture>& tex*/);

	~GLMesh();

	std::vector<VertexPN>	vertices;
	std::vector<GLuint>		indices;
	//std::vector<Texture>	textures;

	void		Init();
	void		Update(float dt);
	void		Render();

	void		SetPosition(const glm::vec3& loc);
	void		SetScale(const glm::vec3& sc);
	void		SetRotation(const glm::vec3& ax, float angle);

	void		Kill();

private:
	GLuint		vao;
	GLuint		vbo;
	GLuint		ibo;
	GLuint		shader;

	GLint		hWorld;				// handle to World matrix
	GLint		hView;				// handle to View matrix
	GLint		hProj;				// handle to projection matrix

	GLint		posAttrib;			// vertex attribute id
	GLint		normalAttrib;		// normal attribute id

	glm::vec3	vecPosition;		// World Space Position
	glm::vec3	vecScale;			// Scale of an object
	glm::vec3	vecRotationAxis;	// rotation axis
	float		m_fAngle;			// angle of rotation

	bool		m_bWireframe;		// wire-frame rendering flag

	glm::mat4   matWorld;		
	glm::mat4	matView;			
	glm::mat4	matProj;

	void		SetupMesh();
};