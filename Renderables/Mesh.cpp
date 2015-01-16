
#include <sstream>
#include "Mesh.h"
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
void	Mesh::Render(GLSLShader shader)
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
		GLint hVar = glGetUniformLocation(shader.GetShaderID(), (name + number).c_str());
		glUniform1f(hVar, i);

		// bind the texture
		glBindTexture(GL_TEXTURE_2D, m_textures[i].id);
	}

	// Draw mesh
	glBindVertexArray(vao);
	glDrawElements(GL_TRIANGLES, m_indices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);

	// set everything back to default
	for (GLuint i = 0; i < m_textures.size(); i++)
	{
		glActiveTexture(GL_TEXTURE0 + i);
		glBindTexture(GL_TEXTURE_2D, 0);
	}
}

