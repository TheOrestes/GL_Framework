
#include <sstream>
#include "Mesh.h"
#include "../Camera/Camera.h"
#include "../Helpers/VertexStructures.h"
#include "../ShaderEngine/GLSLShader.h"
#include "../ObjectSystem/LightsManager.h"
#include "../ObjectSystem/DirectionalLightObject.h"
#include "../ObjectSystem/PointLightObject.h"
#include "GLSkybox.h"

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
void Mesh::PointLightIlluminance(int shaderID)
{
	int numPointLights = LightsManager::getInstance()->GetPointlightsCount();
	glUniform1i(glGetUniformLocation(shaderID, "numPointLights"), numPointLights);

	for (GLuint i = 0 ; i<numPointLights ; ++i)
	{
		PointLightObject* light = LightsManager::getInstance()->GetPointLight(i);
		
		glm::vec3 position = light->GetLightPosition();
		glm::vec4 color    = light->GetLightColor();
		float intensity    = light->GetLightIntensity();
		float radius       = light->GetLightRadius();

		// form a string out of point light Ids
		std::string pointLightPosStr = "pointLights["+ std::to_string(i) + "].position";
		std::string pointLightColStr = "pointLights["+ std::to_string(i) + "].color";
		std::string pointLightIntStr = "pointLights["+ std::to_string(i) + "].intensity";
		std::string pointLightRadStr = "pointLights["+ std::to_string(i) + "].radius";

		glUniform3fv(glGetUniformLocation(shaderID, pointLightPosStr.c_str()), 1, glm::value_ptr(position));
		glUniform4fv(glGetUniformLocation(shaderID,  pointLightColStr.c_str()), 1, glm::value_ptr(color));
		glUniform1f(glGetUniformLocation(shaderID, pointLightIntStr.c_str()), intensity);
		glUniform1f(glGetUniformLocation(shaderID, pointLightRadStr.c_str()), radius);
	}
}

//////////////////////////////////////////////////////////////////////////////////////////
void Mesh::DirectionalLightIlluminance( int shaderID )
{
	int numDirLights = LightsManager::getInstance()->GetDirectionalLightsCount();
	glUniform1i(glGetUniformLocation(shaderID, "numDirLights"), numDirLights);

	for (GLuint i = 0 ; i<numDirLights ; ++i)
	{
		DirectionalLightObject* light = LightsManager::getInstance()->GetDirectionalLight(i);

		glm::vec3 dir	   = light->GetLightDirection();
		glm::vec4 color    = light->GetLightColor();
		float intensity    = light->GetLightIntensity();

		// form a string out of point light Ids
		std::string dirLightPosStr = "dirLights["+ std::to_string(i) + "].direction";
		std::string dirLightColStr = "dirLights["+ std::to_string(i) + "].color";
		std::string dirLightIntStr = "dirLights["+ std::to_string(i) + "].intensity";

		glUniform3fv(glGetUniformLocation(shaderID, dirLightPosStr.c_str()), 1, glm::value_ptr(dir));
		glUniform4fv(glGetUniformLocation(shaderID,  dirLightColStr.c_str()), 1, glm::value_ptr(color));
		glUniform1f(glGetUniformLocation(shaderID, dirLightIntStr.c_str()), intensity);
	}
}

//////////////////////////////////////////////////////////////////////////////////////////
void Mesh::SetShaderVariables( int shaderID, const glm::mat4& world)
{
	//--- Transformation matrices
	glm::mat4 projection = Camera::getInstance().getProjectionMatrix();
	glm::mat4 view = Camera::getInstance().getViewMatrix();

	glm::mat4 InvWorld = glm::inverse(world);
	glm::vec3 CamPosition = Camera::getInstance().getCameraPosition();

	glm::vec3 LightPosition = glm::vec3(0,5,5);

	glUniformMatrix4fv(glGetUniformLocation(shaderID, "matProj"), 1, GL_FALSE, glm::value_ptr(projection));
	glUniformMatrix4fv(glGetUniformLocation(shaderID, "matView"), 1, GL_FALSE, glm::value_ptr(view));
	glUniformMatrix4fv(glGetUniformLocation(shaderID, "matWorld"), 1, GL_FALSE, glm::value_ptr(world));
	glUniformMatrix4fv(glGetUniformLocation(shaderID, "matWorldInv"), 1, GL_FALSE, glm::value_ptr(InvWorld));
	glUniform3fv(glGetUniformLocation(shaderID, "camPosition"), 1, glm::value_ptr(CamPosition));

	// Set Directional light related shader variables...
	DirectionalLightIlluminance(shaderID);

	// Set Point light related shader variables...
	PointLightIlluminance(shaderID);
}	

//////////////////////////////////////////////////////////////////////////////////////////
void	Mesh::Render(GLSLShader* shader, const glm::mat4& world)
{
	GLuint diffuseNr = 1;
	GLuint specularNr = 1;

	shader->Use();

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
		GLuint shaderID = shader->GetShaderID();
		GLint hVar = glGetUniformLocation(shaderID, (name + number).c_str());
		glUniform1i(hVar, i);

		// bind the texture
		glBindTexture(GL_TEXTURE_2D, m_textures[i].id);
	}


	GLuint shaderID = shader->GetShaderID();

	// bind cubemap
	glActiveTexture(GL_TEXTURE2);
	GLint hCubeMap = glGetUniformLocation(shaderID, "texture_cubeMap");
	glUniform1i(hCubeMap, 2);
	GLSkybox::getInstance().BindCubemap();
	

	// Draw mesh
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); 
	glEnable(GL_DEPTH_TEST);


	// Set all Shader variables...
	SetShaderVariables(shaderID, world);

	glBindVertexArray(vao);

	glDrawElements(GL_TRIANGLES, m_indices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
	
	// set everything back to default
	for (GLuint i = 0; i < m_textures.size(); i++)
	{
		glActiveTexture(GL_TEXTURE0 + i);
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	GLSkybox::getInstance().UnbindCubemap();
}

//////////////////////////////////////////////////////////////////////////////////////////


