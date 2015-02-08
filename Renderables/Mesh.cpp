
#include <sstream>
#include "Mesh.h"
#include "../Camera/Camera.h"
#include "../Helpers/VertexStructures.h"
#include "../ShaderEngine/GLSLShader.h"


//////////////////////////////////////////////////////////////////////////////////////////
Mesh::Mesh(std::vector<VertexPNT> vertices, std::vector<GLuint> indices, std::vector<Texture> textures)
{
	m_vertices = vertices;
	m_indices = indices;
	m_textures = textures;

	SetupMesh();
}

//////////////////////////////////////////////////////////////////////////////////////////
Mesh::~Mesh()
{

}

//////////////////////////////////////////////////////////////////////////////////////////
void	Mesh::SetupMesh()
{
	// vertex array buffer
	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);
	glGenBuffers(1, &ibo);

	glBindVertexArray(vao);

	// vertex buffer
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, m_vertices.size() * sizeof(VertexPNT), &m_vertices[0], GL_STATIC_DRAW);

	// index buffer
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_indices.size() * sizeof(GLuint), &m_indices[0], GL_STATIC_DRAW);

	// vertex position
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(VertexPNT), (void*)0);

	// normal 
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(VertexPNT), (void*)(offsetof(VertexPNT, normal)));

	// texture
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(VertexPNT), (void*)(offsetof(VertexPNT, texcoord)));

	glBindVertexArray(0);
}

//////////////////////////////////////////////////////////////////////////////////////////
void	Mesh::Render(GLSLShader* shader, const glm::mat4& world)
{
	GLuint diffuseNr = 1;
	GLuint specularNr = 1;

	for (GLuint i = 0 ; i < m_textures.size() ; i++)
	{

		glActiveTexture(GL_TEXTURE0 + i);

		std::stringstream ss;
		std::string number;
		std::string name = m_textures[i].name;

		if(name == "texture_diffuse")
		{
			ss << diffuseNr++;
		}
		else if(name == "texture_specular")
		{
			ss << specularNr++;
		}

		number = ss.str();

		// set sampler to the correct texture
		GLint hVar = glGetUniformLocation(shader->GetShaderID(), (name + number).c_str());
		glUniform1i(hVar, i);

		// bind the texture
		glBindTexture(GL_TEXTURE_2D, m_textures[i].id);
	}

	// Draw mesh
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); 
	glEnable(GL_DEPTH_TEST);

	glBindVertexArray(vao);

	// transformation
	GLuint shaderID = shader->GetShaderID();

	shader->Use();

	//--- Transformation matrices
	glm::mat4 projection = Camera::getInstance().getProjectionMatrix();
	glm::mat4 view = Camera::getInstance().getViewMatrix();

	glm::mat4 InvWorld = glm::inverse(world);
	glm::vec3 CamPosition = Camera::getInstance().getCameraPosition();

	glm::vec3 LightPosition = glm::vec3(0.0f, 0.0f, 25.0f);

	glUniformMatrix4fv(glGetUniformLocation(shaderID, "matProj"), 1, GL_FALSE, glm::value_ptr(projection));
	glUniformMatrix4fv(glGetUniformLocation(shaderID, "matView"), 1, GL_FALSE, glm::value_ptr(view));
	glUniformMatrix4fv(glGetUniformLocation(shaderID, "matWorld"), 1, GL_FALSE, glm::value_ptr(world));
	glUniformMatrix4fv(glGetUniformLocation(shaderID, "matWorldInv"), 1, GL_FALSE, glm::value_ptr(InvWorld));
	glUniform3fv(glGetUniformLocation(shaderID, "camPosition"), 1, glm::value_ptr(CamPosition));
	glUniform3fv(glGetUniformLocation(shaderID, "LightPosition"), 1, glm::value_ptr(LightPosition));
	//---

	glDrawElements(GL_TRIANGLES, m_indices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
	
	// set everything back to default
	for (GLuint i = 0; i < m_textures.size(); i++)
	{
		glActiveTexture(GL_TEXTURE0 + i);
		glBindTexture(GL_TEXTURE_2D, 0);
	}
}

