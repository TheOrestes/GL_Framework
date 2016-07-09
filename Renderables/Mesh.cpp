
#include <sstream>
#include "Mesh.h"
#include "../Camera/Camera.h"
#include "../Helpers/VertexStructures.h"
#include "../ShaderEngine/GLSLShader.h"
#include "../MaterialSystem/Material.h"
#include "../ObjectSystem/LightsManager.h"
#include "../ObjectSystem/DirectionalLightObject.h"
#include "../ObjectSystem/PointLightObject.h"
#include "GLSkybox.h"

//////////////////////////////////////////////////////////////////////////////////////////
Mesh::Mesh(std::vector<VertexPNTBT> vertices, std::vector<GLuint> indices, std::vector<Texture> textures)
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
	glBufferData(GL_ARRAY_BUFFER, m_vertices.size() * sizeof(VertexPNTBT), &m_vertices[0], GL_STATIC_DRAW);

	// index buffer
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_indices.size() * sizeof(GLuint), &m_indices[0], GL_STATIC_DRAW);

	// vertex position
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(VertexPNTBT), (void*)0);

	// normal 
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(VertexPNTBT), (void*)(offsetof(VertexPNTBT, normal)));

	// tangent
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(VertexPNTBT), (void*)(offsetof(VertexPNTBT, tangent)));

	// binormal
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(VertexPNTBT), (void*)(offsetof(VertexPNTBT, binormal)));

	// texture
	glEnableVertexAttribArray(4);
	glVertexAttribPointer(4, 2, GL_FLOAT, GL_FALSE, sizeof(VertexPNTBT), (void*)(offsetof(VertexPNTBT, texcoord)));

	glBindVertexArray(0);
}

//////////////////////////////////////////////////////////////////////////////////////////
void Mesh::SetMaterialProperties(int shaderID, Material* mat)
{
	glUniform4fv(glGetUniformLocation(shaderID, "material.Albedo"), 1, glm::value_ptr(mat->m_colAlbedo));
	glUniform4fv(glGetUniformLocation(shaderID, "material.Specular"), 1, glm::value_ptr(mat->m_colSpecular));
	glUniform4fv(glGetUniformLocation(shaderID, "material.Emission"), 1, glm::value_ptr(mat->m_colEmissive));
}

//////////////////////////////////////////////////////////////////////////////////////////
void Mesh::SetShaderVariables( int shaderID, const glm::mat4& world, Material* mat)
{
	//--- Transformation matrices
	glm::mat4 projection = Camera::getInstance().getProjectionMatrix();
	glm::mat4 view = Camera::getInstance().getViewMatrix();

	glm::vec3 CamPosition = Camera::getInstance().getCameraPosition();
	glUniform3fv(glGetUniformLocation(shaderID, "camPosition"), 1, glm::value_ptr(CamPosition));

	glUniformMatrix4fv(glGetUniformLocation(shaderID, "matProj"), 1, GL_FALSE, glm::value_ptr(projection));
	glUniformMatrix4fv(glGetUniformLocation(shaderID, "matView"), 1, GL_FALSE, glm::value_ptr(view));
	glUniformMatrix4fv(glGetUniformLocation(shaderID, "matWorld"), 1, GL_FALSE, glm::value_ptr(world));

	// set material properties associated with this mesh...
	SetMaterialProperties(shaderID, mat);
}	

//////////////////////////////////////////////////////////////////////////////////////////
void	Mesh::Render(GLSLShader* shader, const glm::mat4& world, Material* mat)
{
	GLuint diffuseNr = 1;		bool bDiffuseTexture = false;
	GLuint specularNr = 1;		bool bSpecularTexture = false;
	GLuint normalNr = 1;		bool bNormalMapTexture = false;
	GLuint emissNr = 1;			bool bEmissiveTexture = false;
	GLuint heightNr = 1;		bool bHeightMapTexture = false;
	GLuint ambientNr = 1;		bool bAmbientOccTexture = false;
	GLuint shininessNr = 1;		bool bShininessTexture = false;
	GLuint dispNr = 1;			bool bDisplacementTexture = false;
	GLuint lightmapNr = 1;		bool bLightMapTexture = false;
	GLuint reflectionNr = 1;	bool bReflectionTexture = false;

	shader->Use();

	// set sampler to the correct texture
	GLuint shaderID = shader->GetShaderID();

	GLuint i = 0;
	for ( ; i < m_textures.size() ; i++)
	{
		glActiveTexture(GL_TEXTURE0 + i);

		std::stringstream ss;
		std::string number;
		std::string name = m_textures[i].name;

		if(name == "texture_diffuse")
		{
			bDiffuseTexture = true;
			glUniform1i(glGetUniformLocation(shaderID, "bDiffuseTexture"), bDiffuseTexture);

			ss << diffuseNr++;
		}
		else if(name == "texture_specular")
		{
			bSpecularTexture = true;
			glUniform1i(glGetUniformLocation(shaderID, "bSpecularTexture"), bSpecularTexture);

			ss << specularNr++;
		}
		else if (name == "texture_normal")
		{
			bNormalMapTexture = true;
			glUniform1i(glGetUniformLocation(shaderID, "bNormalMapTexture"), bNormalMapTexture);

			ss << normalNr++;
		}
		else if(name == "texture_emissive")
		{
			bEmissiveTexture = true;
			glUniform1i(glGetUniformLocation(shaderID, "bEmissiveTexture"), bEmissiveTexture);

			ss << emissNr++;
		}
		else if(name == "texture_height")
		{
			bHeightMapTexture = true;
			glUniform1i(glGetUniformLocation(shaderID, "bHeightMapTexture"), bHeightMapTexture);

			ss << heightNr++;
		}
		else if(name == "texture_ambient")
		{
			bAmbientOccTexture = true;
			glUniform1i(glGetUniformLocation(shaderID, "bAmbientOccTexture"), bAmbientOccTexture);

			ss << ambientNr++;
		}
		else if(name == "texture_shininess")
		{
			bShininessTexture = true;
			glUniform1i(glGetUniformLocation(shaderID, "bShininessTexture"), bShininessTexture);

			ss << shininessNr++;
		}
		else if(name == "texture_displacement")
		{
			bDisplacementTexture = true;
			glUniform1i(glGetUniformLocation(shaderID, "bDisplacementTexture"), bDisplacementTexture);

			ss << dispNr++;
		}
		else if(name == "texture_lightmap")
		{
			bLightMapTexture = true;
			glUniform1i(glGetUniformLocation(shaderID, "bLightMapTexture"), bLightMapTexture);

			ss << lightmapNr++;
		}
		else if(name == "texture_reflection")
		{
			bReflectionTexture = true;
			glUniform1i(glGetUniformLocation(shaderID, "bReflectionTexture"), bReflectionTexture);

			ss << reflectionNr++;
		}

		number = ss.str();
		GLint hVar = glGetUniformLocation(shaderID, (name + number).c_str());
		glUniform1i(hVar, i);

		// bind the texture
		glBindTexture(GL_TEXTURE_2D, m_textures[i].id);
	}

	// Draw mesh
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); 
	glEnable(GL_DEPTH_TEST);


	// Set all Shader variables...
	SetShaderVariables(shaderID, world, mat);

	glBindVertexArray(vao);

	glDrawElements(GL_TRIANGLES, m_indices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);

	//glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	
	// set everything back to default
	for (GLuint i = 0; i < m_textures.size(); i++)
	{
		glActiveTexture(GL_TEXTURE0 + i);
		glBindTexture(GL_TEXTURE_2D, 0);
	}
}

//////////////////////////////////////////////////////////////////////////////////////////


