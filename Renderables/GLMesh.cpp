
#include "GLMesh.h"
#include "../ShaderEngine/GLSLParser.h"
#include "../Camera/Camera.h"

/////////////////////////////////////////////////////////////////////////////////////////
GLMesh::GLMesh()
	: vao(0),
	  vbo(0),
	  ibo(0),
	  shader(0)
{
	vertices.clear();
	indices.clear();
	//textures.clear();

	SetPosition(glm::vec3(0,0,0));
	SetScale(glm::vec3(1,1,1));
	SetRotation(glm::vec3(0,1,0), 0.0f);

	m_bWireframe = true;
}

//////////////////////////////////////////////////////////////////////////////////////////
GLMesh::GLMesh(const std::vector<VertexPN>& vert, const std::vector<GLuint>& ind/*, const std::vector<Texture>& tex*/)
{
	vertices = vert;
	indices = ind;
	//textures = tex;

	SetPosition(glm::vec3(0,0,0));
	SetScale(glm::vec3(1,1,1));
	SetRotation(glm::vec3(0,1,0), 0.0f);

	m_bWireframe = true;
}

//////////////////////////////////////////////////////////////////////////////////////////
GLMesh::~GLMesh()
{
	Kill();
}

//////////////////////////////////////////////////////////////////////////////////////////
void GLMesh::SetPosition(const glm::vec3& loc)
{
	vecPosition = loc;
}

//////////////////////////////////////////////////////////////////////////////////////////
void GLMesh::SetScale(const glm::vec3& sc)
{
	vecScale = sc;
}

//////////////////////////////////////////////////////////////////////////////////////////
void GLMesh::SetRotation( const glm::vec3& ax, float angle )
{
	vecRotationAxis = ax;
	m_fAngle = angle;
}

//////////////////////////////////////////////////////////////////////////////////////////
void GLMesh::Init()
{
	// This function was created for the consistency purpose similar toW function calls of 
	// GLCube.
	SetupMesh();
}

//////////////////////////////////////////////////////////////////////////////////////////
void	GLMesh::SetupMesh()
{
	// Create shader object
	shader = GLSLParser::getInstance().LoadShader("Shaders/vsLighting.glsl", "Shaders/psLighting.glsl");

	glGenBuffers(1, &vao);
	glGenBuffers(1, &vbo);
	glGenBuffers(1, &ibo);

	glBindVertexArray(vao);

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(VertexPN), &vertices[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), &indices[0], GL_STATIC_DRAW);
	
	// Vertex position
	posAttrib = glGetAttribLocation(shader, "in_Position");
	glEnableVertexAttribArray(posAttrib);
	glVertexAttribPointer(posAttrib, 3, GL_FLOAT, false, sizeof(VertexPN), (void*)0);

	// normals
	normalAttrib = glGetAttribLocation(shader, "in_Normal");
	glEnableVertexAttribArray(normalAttrib);
	glVertexAttribPointer(normalAttrib, 3, GL_FLOAT, false, sizeof(VertexPN), (void*)sizeof(glm::vec3));

	// textures
	//glEnableVertexAttribArray(2);
	//glVertexAttribPointer(2, 2, GL_FLOAT, false, sizeof(VertexPNT), (void*)sizeof(glm::vec3));

	hWorld = glGetUniformLocation(shader, "matWorld");
	hView = glGetUniformLocation(shader, "matView");
	hProj = glGetUniformLocation(shader, "matProj");

	glBindVertexArray(0);

	// WIREFRAME MODE!
	if(m_bWireframe)
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); 
}

//////////////////////////////////////////////////////////////////////////////////////////
void GLMesh::Update( float dt )
{
	static float angle = 0.0f;
	angle += dt;

	// create world transformation matrix
	// So what we are doing is, basically glm::translate/rotate etc matrices take first param
	// as an argument which usually is kept to be identity matrix. Internally, these functions,
	// multiply with the first argument.
	// https://www.youtube.com/watch?v=U_RtSchYYec

	glm::mat4 T   = glm::translate(glm::mat4(1), vecPosition);
	glm::mat4 TR  = glm::rotate(T, angle, glm::vec3(0.0f, 1.0f, 0.0f));
	glm::mat4 TRS = glm::scale(TR, glm::vec3(vecScale.x, vecScale.y, vecScale.z));

	// Set final World transformation matrix...
	matWorld = TRS;

	// Create composite ViewProjection matrix
	//matWorld = glm::rotate(matWorld, 0.1f*dt, glm::vec3(0.0f, 1.0f, 0.0f));
	matView = Camera::getInstance().getViewMatrix();
	matProj = Camera::getInstance().getProjectionMatrix();
}

//////////////////////////////////////////////////////////////////////////////////////////
void	GLMesh::Render()
{
	glUseProgram(shader);
	glBindVertexArray(vao);	

	// Set uniform variables
	glUniformMatrix4fv(hWorld, 1, GL_FALSE, glm::value_ptr(matWorld));
	glUniformMatrix4fv(hView, 1, GL_FALSE, glm::value_ptr(matView));
	glUniformMatrix4fv(hProj, 1, GL_FALSE, glm::value_ptr(matProj));

	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
	
	glBindVertexArray(0);
}

//////////////////////////////////////////////////////////////////////////////////////////
void GLMesh::Kill()
{
	glDeleteProgram(shader);
	glDeleteBuffers(1, &vbo);
	glDeleteBuffers(1, &ibo);
	glDeleteVertexArrays(1, &vao);
}



