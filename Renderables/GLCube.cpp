
#include "GLCube.h"
#include "..\ShaderEngine\GLSLParser.h"
#include "..\Camera\Camera.h"
#include "..\Helpers\VertexStructures.h"

//////////////////////////////////////////////////////////////////////////////////////////
GLCube::GLCube()
{
	// Vertex Data
	vertices[0] = VertexPC(glm::vec3(-1,-1,1), glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));
	vertices[1] = VertexPC(glm::vec3(1,-1,1), glm::vec4(0.0f, 1.0f, 0.0f, 1.0f));
	vertices[2] = VertexPC(glm::vec3(1,1,1), glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));
	vertices[3] = VertexPC(glm::vec3(-1,1,1), glm::vec4(1.0f, 0.0f, 1.0f, 1.0f));
	vertices[4] = VertexPC(glm::vec3(-1,-1,-1), glm::vec4(0.0f, 1.0f, 1.0f, 1.0f));
	vertices[5] = VertexPC(glm::vec3(1,-1,-1), glm::vec4(1.0f, 1.0f, 0.0f, 1.0f));
	vertices[6] = VertexPC(glm::vec3(1,1,-1), glm::vec4(1.0f, 0.0f, 1.0f, 1.0f));
	vertices[7] = VertexPC(glm::vec3(-1,1,-1), glm::vec4(0.0f, 1.0f, 1.0f, 1.0f));

	// Index data ( 6 faces = 12 triangles = 36 indices )

	indices[0] = 0;				indices[1] = 1;			indices[2] = 2;
	indices[3] = 2;				indices[4] = 3;			indices[5] = 0;

	indices[6] = 3;				indices[7] = 2;			indices[8] = 6;
	indices[9] = 6;				indices[10] = 7;		indices[11] = 3;

	indices[12] = 7;			indices[13] = 6;		indices[14] = 5;
	indices[15] = 5;			indices[16] = 4;		indices[17] = 7;

	indices[18] = 4;			indices[19] = 5;		indices[20] = 1;
	indices[21] = 1;			indices[22] = 0;		indices[23] = 4;

	indices[24] = 4;			indices[25] = 0;		indices[26] = 3;
	indices[27] = 3;			indices[28] = 7;		indices[29] = 4;

	indices[30] = 1;			indices[31] = 5;		indices[32] = 6;
	indices[33] = 6;			indices[34] = 2;		indices[35] = 1;
}

//////////////////////////////////////////////////////////////////////////////////////////
GLCube::~GLCube()
{

}

//////////////////////////////////////////////////////////////////////////////////////////
void GLCube::SetupViewProjMatrix()
{
	hWVP = glGetUniformLocation(shader, "matWVP");
}

//////////////////////////////////////////////////////////////////////////////////////////
void GLCube::Init()
{
	// Create shader object
	shader = GLSLParser::getInstance().LoadShader("Shaders/vs.glsl", "Shaders/ps.glsl");

	// create vao
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	// create vbo
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, 8 * sizeof(VertexPC), vertices, GL_STATIC_DRAW);

	// create ibo
	glGenBuffers(1, &ibo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	posAttrib = glGetAttribLocation(shader, "position");
	glEnableVertexAttribArray(posAttrib);
	glVertexAttribPointer(posAttrib, 3, GL_FLOAT, false, sizeof(VertexPC), nullptr);

	colAttrib = glGetAttribLocation(shader, "inColor");
	glEnableVertexAttribArray(colAttrib);
	glVertexAttribPointer(colAttrib, 4, GL_FLOAT, false, sizeof(VertexPC), (void*)sizeof(sizeof(glm::vec3)));

	SetupViewProjMatrix();

	glEnable(GL_DEPTH_TEST);
}

//////////////////////////////////////////////////////////////////////////////////////////
void GLCube::Update(float dt)
{
	// Create composite ViewProjection matrix
	matWorld = glm::rotate(matWorld, 0.1f*dt, glm::vec3(0.0f, 1.0f, 0.0f));
	matView = Camera::getInstance().getViewMatrix(); //glm::lookAt(glm::vec3(0.0f, 2.0f, 5.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	matProj = Camera::getInstance().getProjectionMatrix(); //glm::perspective(45.0f, 1.33f, 0.1f, 10.0f);

	matWVP = matProj * matView * matWorld;
}

//////////////////////////////////////////////////////////////////////////////////////////
void GLCube::Render()
{
	glUseProgram(shader);
	//glBindVertexArray(vao);

	glUniformMatrix4fv(hWVP, 1, GL_FALSE, glm::value_ptr(matWVP));

	// know more about the last element!
	// http://stackoverflow.com/questions/17191258/no-display-from-gldrawelements
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
}

//////////////////////////////////////////////////////////////////////////////////////////
void GLCube::Kill()
{
	glDeleteProgram(shader);
	glDeleteBuffers(1, &vbo);
	glDeleteBuffers(1, &ibo);
	glDeleteVertexArrays(1, &vao);
}