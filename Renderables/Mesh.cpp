
#include <sstream>
#include "Mesh.h"
#include "../Camera/Camera.h"
#include "../Helpers/VertexStructures.h"
#include "../ShaderEngine/GLSLShader.h"
#include "../MaterialSystem/Material.h"
#include "../Helpers/TextureManager.h"
#include "../ObjectSystem/LightsManager.h"
#include "../ObjectSystem/DirectionalLightObject.h"
#include "../ObjectSystem/PointLightObject.h"
#include "GLSkybox.h"

//////////////////////////////////////////////////////////////////////////////////////////
Mesh::Mesh(std::vector<VertexPNTBT> vertices, std::vector<GLuint> indices)
{
	m_vertices = vertices;
	m_indices = indices;

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
	glUniform4fv(glGetUniformLocation(shaderID, "material.Emissive"), 1, glm::value_ptr(mat->m_colEmissive));
	glUniform1fv(glGetUniformLocation(shaderID, "material.Roughness"), 1, &(mat->m_fRoughness));
	glUniform1fv(glGetUniformLocation(shaderID, "material.Metallic"), 1, &(mat->m_fMetallic));
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
	if (!mat || !shader)
		return;

	bool bDiffuseTexture = false;
	bool bSpecularTexture = false;
	bool bNormalMapTexture = false;
	bool bEmissiveTexture = false;
	bool bHeightMapTexture = false;
	bool bAmbientOccTexture = false;
	bool bEnvironmentTexture = false;

	shader->Use();

	// set sampler to the correct texture
	GLuint shaderID = shader->GetShaderID();

	if (mat->m_pTexAlbedo.hasChanged())
	{
		bDiffuseTexture = true;

		// Remove current texture ID from the graphics memory!
		GLuint currTexID = mat->m_pTexAlbedo.getID();
		glDeleteTextures(1, &currTexID);

		// Load new texture...
		mat->m_pTexAlbedo.setID(TextureManager::getInstannce().LoadTextureFromFile(mat->m_pTexAlbedo.getPath()));
		mat->m_pTexAlbedo.changed = false;
		
		glUniform1i(glGetUniformLocation(shaderID, "bDiffuseTexture"), bDiffuseTexture);
		glUniform1i(glGetUniformLocation(shaderID, "texture_diffuse"), 0);
	}

	if (mat->m_pTexEmission.hasChanged())
	{
		bEmissiveTexture = true;
		GLuint currTexID = mat->m_pTexEmission.getID();
		glDeleteTextures(1, &currTexID);
		mat->m_pTexEmission.setID(TextureManager::getInstannce().LoadTextureFromFile(mat->m_pTexEmission.getPath()));
		mat->m_pTexEmission.changed = false;

		glUniform1i(glGetUniformLocation(shaderID, "bEmissiveTexture"), bEmissiveTexture);
		glUniform1i(glGetUniformLocation(shaderID, "texture_emissive"), 1);
	}

	if (mat->m_pTexHeight.hasChanged())
	{
		bHeightMapTexture = true;
		GLuint currTexID = mat->m_pTexHeight.getID();
		glDeleteTextures(1, &currTexID);
		mat->m_pTexHeight.setID(TextureManager::getInstannce().LoadTextureFromFile(mat->m_pTexHeight.getPath()));
		mat->m_pTexHeight.changed = false;

		glUniform1i(glGetUniformLocation(shaderID, "bHeightMapTexture"), bHeightMapTexture);
		glUniform1i(glGetUniformLocation(shaderID, "texture_height"), 2);
	}
	
	if (mat->m_pTexNormal.hasChanged())
	{
		bNormalMapTexture = true;
		GLuint currTexID = mat->m_pTexNormal.getID();
		glDeleteTextures(1, &currTexID);
		mat->m_pTexNormal.setID(TextureManager::getInstannce().LoadTextureFromFile(mat->m_pTexNormal.getPath()));
		mat->m_pTexNormal.changed = false;

		glUniform1i(glGetUniformLocation(shaderID, "bNormalMapTexture"), bHeightMapTexture);
		glUniform1i(glGetUniformLocation(shaderID, "texture_normal"), 3);
	}

	if (mat->m_pTexOcclusion.hasChanged())
	{
		bAmbientOccTexture = true;
		GLuint currTexID = mat->m_pTexOcclusion.getID();
		glDeleteTextures(1, &currTexID);
		mat->m_pTexOcclusion.setID(TextureManager::getInstannce().LoadTextureFromFile(mat->m_pTexOcclusion.getPath()));
		mat->m_pTexOcclusion.changed = false;

		glUniform1i(glGetUniformLocation(shaderID, "bAmbientOccTexture"), bAmbientOccTexture);
		glUniform1i(glGetUniformLocation(shaderID, "texture_ambient"), 4);
	}

	if (mat->m_pTexSpecular.hasChanged())
	{
		bSpecularTexture = true;
		GLuint currTexID = mat->m_pTexSpecular.getID();
		glDeleteTextures(1, &currTexID);
		mat->m_pTexSpecular.setID(TextureManager::getInstannce().LoadTextureFromFile(mat->m_pTexSpecular.getPath()));
		mat->m_pTexSpecular.changed = false;

		glUniform1i(glGetUniformLocation(shaderID, "bSpecularTexture"), bSpecularTexture);
		glUniform1i(glGetUniformLocation(shaderID, "texture_specular"), 5);
	}

	// Get active Skybox
	glUniform1i(glGetUniformLocation(shaderID, "texture_environment"), 6);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, mat->m_pTexAlbedo.getID());

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, mat->m_pTexEmission.getID());

	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, mat->m_pTexHeight.getID());

	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D, mat->m_pTexNormal.getID());

	glActiveTexture(GL_TEXTURE4);
	glBindTexture(GL_TEXTURE_2D, mat->m_pTexOcclusion.getID());

	glActiveTexture(GL_TEXTURE5);
	glBindTexture(GL_TEXTURE_2D, mat->m_pTexSpecular.getID());

	// Set active skybox texture
	glActiveTexture(GL_TEXTURE6);
	glBindTexture(GL_TEXTURE_2D, GLSkybox::getInstance().GetEnvironmentMapID());

	/*GLuint i = 0;
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
	}*/

	// Draw mesh
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); 
	glEnable(GL_DEPTH_TEST);

	// Set all Shader variables...
	SetShaderVariables(shaderID, world, mat);

	glBindVertexArray(vao);

	glDrawElements(GL_TRIANGLES, m_indices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);

	/*glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, 0);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, 0);

	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, 0);

	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D, 0);

	glActiveTexture(GL_TEXTURE4);
	glBindTexture(GL_TEXTURE_2D, 0);

	glActiveTexture(GL_TEXTURE5);
	glBindTexture(GL_TEXTURE_2D, 0);*/

	//glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	
	// set everything back to default
	/*for (GLuint i = 0; i < m_textures.size(); i++)
	{
		glActiveTexture(GL_TEXTURE0 + i);
		glBindTexture(GL_TEXTURE_2D, 0);
	}*/
}

//////////////////////////////////////////////////////////////////////////////////////////


