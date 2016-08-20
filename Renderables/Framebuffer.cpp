
#include "FrameBuffer.h"
#include "../ShaderEngine/GLSLShader.h"
#include "../Helpers/VertexStructures.h"
#include "../UI/UIManager.h"
#include "../ObjectSystem/LightsManager.h"
#include "../ObjectSystem/PointLightObject.h"
#include "../ObjectSystem/DirectionalLightObject.h"
#include "../Camera/Camera.h"
#include "../Helpers/LogManager.h"
#include <iostream>

#define BLOOM_ENABLED 0

//////////////////////////////////////////////////////////////////////////////////////////
Framebuffer::Framebuffer()
{
	m_pGenericPostFX = nullptr;
	m_pBlurPostFX = nullptr;
	m_pBloomPostFX = nullptr;
	m_pDeferredLightingFX = nullptr;

	// Screen aligned Quad setup
	quadVertices[0] = VertexPT(glm::vec3(-1,1,0), glm::vec2(0,1));
	quadVertices[1] = VertexPT(glm::vec3(-1,-1,0), glm::vec2(0,0));
	quadVertices[2] = VertexPT(glm::vec3(1,-1,0), glm::vec2(1,0));
	quadVertices[3] = VertexPT(glm::vec3(-1,1,0), glm::vec2(0,1));
	quadVertices[4] = VertexPT(glm::vec3(1,-1,0), glm::vec2(1,0));
	quadVertices[5] = VertexPT(glm::vec3(1,1,0), glm::vec2(1,1));

	// FX Data
	m_pFXData = nullptr;
	m_pFXUI = nullptr;
}

//////////////////////////////////////////////////////////////////////////////////////////
Framebuffer::~Framebuffer()
{
	delete m_pGenericPostFX;
	delete m_pDeferredLightingFX;
	delete m_pBlurPostFX;
	delete m_pBloomPostFX;
	delete m_pFXData;
	delete m_pFXUI;
}

//////////////////////////////////////////////////////////////////////////////////////////
void Framebuffer::FramebufferSetup()
{
	// create actual framebuffer object & bind it. 
	glGenFramebuffers(1, &fbo);
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);

		// create two texture buffer objects 
		// 1. Position buffer
		// 2. Normal buffer
		// 3. Albedo color buffer
		// 4. Cubemap buffer
		glGenTextures(MAX_NUM_BUFFER, tbo);
		for (int i = 0 ; i < MAX_NUM_BUFFER ; i++)
		{
			glBindTexture(GL_TEXTURE_2D, tbo[i]);

			if( i != ALBEDO_COLOR_BUFFER )
				glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, 1280, 800, 0, GL_RGB, GL_FLOAT, NULL);
			else
				glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 1280, 800, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);	

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); 
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

			// attach texture object to already bind framebuffer as color attachment
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, tbo[i], 0);			
		}
		
		// create render buffer object
		glGenRenderbuffers(1, &rbo);
		glBindRenderbuffer(GL_RENDERBUFFER, rbo);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, 1280, 800);
		glBindRenderbuffer(GL_RENDERBUFFER, 0);

		// attach render buffer object as depth & stencil attachment to framebuffer
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);

		// Tell OGL which color attachments we will use...
		GLuint attachments[MAX_NUM_BUFFER] = {GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3};
		glDrawBuffers(MAX_NUM_BUFFER, attachments);

		// Finally, check if framebuffer is complete
		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		{
			//std::cout << FRED("Frame Buffer status error!") << std::endl;
			return;
		}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

#if BLOOM_ENABLED

	// Courtesy : http://www.learnopengl.com/#!Advanced-Lighting/Bloom
	// Ping pong framebuffer for Blurring
	glGenFramebuffers(2, bloomFBO);
	glGenTextures(2, bloomColorBuffer);

	for (GLuint i = 0 ; i < 2 ; i++)
	{
		glBindFramebuffer(GL_FRAMEBUFFER, bloomFBO[i]);
		glBindTexture(GL_TEXTURE_2D, bloomColorBuffer[i]);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, 1280, 800, 0, GL_RGB, GL_FLOAT, NULL);
		//glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 1280, 800, 0, GL_RGB, GL_UNSIGNED_INT, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); 
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); // We clamp to the edge as the blur filter would otherwise sample repeated texture values!
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 , GL_TEXTURE_2D, 
								bloomColorBuffer[i], 0);
	}
#endif

	// Load all required shaders for post processing...
	m_pGenericPostFX = new GLSLShader("Shaders/vsFramebuffer.glsl", "Shaders/psFramebuffer.glsl");
	m_pDeferredLightingFX = new GLSLShader("Shaders/vsFramebuffer.glsl", "Shaders/psDeferredLighting.glsl");
	m_pBlurPostFX = new GLSLShader("Shaders/vsFramebuffer.glsl", "Shaders/psBlur.glsl");
	m_pBloomPostFX = new GLSLShader("shaders/vsFramebuffer.glsl", "Shaders/psFramebuffer.glsl");

	//-------------------------- Create screen-aligned Quad 
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

		glGenBuffers(1, &vbo);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(GL_ARRAY_BUFFER, 6*sizeof(VertexPT), quadVertices, GL_STATIC_DRAW);

		posAttrib = glGetAttribLocation(m_pDeferredLightingFX->GetShaderID(), "in_Position");
		glEnableVertexAttribArray(posAttrib);
		glVertexAttribPointer(posAttrib, 3, GL_FLOAT, GL_FALSE, sizeof(VertexPT), (void*)0);

		texAttrib = glGetAttribLocation(m_pDeferredLightingFX->GetShaderID(), "in_TexCoord");
		glEnableVertexAttribArray(texAttrib);
		glVertexAttribPointer(texAttrib, 2, GL_FLOAT, GL_FALSE, sizeof(VertexPT), (void*)offsetof(VertexPT, uv));

	glBindVertexArray(0);

	// FX Data initialize
	m_pFXData = new PostFXData();

	// Initialize FX UI
	m_pFXUI = TwNewBar("Postprocess");
	TwAddVarRW(m_pFXUI, "Bloom", TW_TYPE_BOOL8, &(m_pFXData->m_bBloomOn), "label='Bloom Control'");
	TwAddVarRW(m_pFXUI, "Exposure", TW_TYPE_FLOAT, &(m_pFXData->m_fExposure), 
						"label='Exposure Control' min=0 max=10 step=0.1");
	TwAddVarRW(m_pFXUI, "BlurIter", TW_TYPE_INT8, &(m_pFXData->m_iBlurIter), 
						"label='Blur Iteration' min=0 max=5 step=1");
}

//////////////////////////////////////////////////////////////////////////////////////////
void Framebuffer::BeginRenderToFramebuffer()
{
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);

	glClearColor(0.01f, 0.01f, 0.01f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glEnable(GL_DEPTH_TEST);
}

//////////////////////////////////////////////////////////////////////////////////////////
void Framebuffer::EndRenderToFramebuffer()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

//////////////////////////////////////////////////////////////////////////////////////////
void Framebuffer::RenderDeferredLightingPass()
{
	glClearColor(0.01f, 0.01f, 0.01f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	// No need of any depth testing while rendering a single scene aligned quad...
	glDisable(GL_DEPTH_TEST);

	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	m_pDeferredLightingFX->Use();

	int id = m_pDeferredLightingFX->GetShaderID();
	
	SetShaderVariables(id);

	// Bind all the data buffers as textures in deferred lighting pass...
	for (int i = 0 ; i < MAX_NUM_BUFFER ; i++)
	{
		glActiveTexture(GL_TEXTURE0 + i);
		glBindTexture(GL_TEXTURE_2D, tbo[i]);
	}
	
	glBindVertexArray(vao);
	
	glDrawArrays(GL_TRIANGLES, 0, 6);

	glBindVertexArray(0);
}

//////////////////////////////////////////////////////////////////////////////////////////
void Framebuffer::BlurPass()
{
	horizontal = true;
	bool first_iter = true;

	m_pBlurPostFX->Use();

	for (int i = 0 ; i<2 * m_pFXData->m_iBlurIter ; i++)
	{
		glBindFramebuffer(GL_FRAMEBUFFER, bloomFBO[horizontal]);
		glUniform1i(glGetUniformLocation(m_pBlurPostFX->GetShaderID(), "horizontal"), horizontal);

		if (first_iter)
		{
			// First iteration requires brightness texture as input as both pingpongbuffer
			// will be empty...
			glBindTexture(GL_TEXTURE_2D, tbo[1]);
		}
		else
		{
			// For all iterations other than first, we choose other framebuffer's color
			// buffer for uniform blur in both horizontal & vertical direction...
			glBindTexture(GL_TEXTURE_2D, bloomColorBuffer[!horizontal]);
		}

		// Draw Quad...
		glBindVertexArray(vao);
		glDrawArrays(GL_TRIANGLES, 0, 6);
		glBindVertexArray(0);

		// setup for next iteration...
		horizontal = !horizontal;
		if(first_iter)
			first_iter = false;
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

//////////////////////////////////////////////////////////////////////////////////////////
void Framebuffer::BlendPass()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	m_pBloomPostFX->Use();

	// Original Scene rendered to texture
	glActiveTexture(GL_TEXTURE0);
	GLint hVar1 = glGetUniformLocation(m_pBloomPostFX->GetShaderID(), "screenTexture");
	glUniform1i(hVar1, 0);
	glBindTexture(GL_TEXTURE_2D, tbo[0]);

	// Blurred scene rendered to texture
	glActiveTexture(GL_TEXTURE1);
	GLint hVar2 = glGetUniformLocation(m_pBloomPostFX->GetShaderID(), "blurTexture");
	glUniform1i(hVar2, 1);
	glBindTexture(GL_TEXTURE_2D, bloomColorBuffer[!horizontal]);

	glUniform1i(glGetUniformLocation(m_pBloomPostFX->GetShaderID(), "bloomEnabled"), m_pFXData->m_bBloomOn);
	glUniform1f(glGetUniformLocation(m_pGenericPostFX->GetShaderID(), "exposure"), m_pFXData->m_fExposure);
	// Draw Quad...
	glBindVertexArray(vao);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glBindVertexArray(0);
}

void Framebuffer::RenderBloomEffect()
{
	// Bloom controlled by UI?
	if(m_pFXData->m_bBloomOn)
	{
		BlurPass();
		BlendPass();
	}
}

//////////////////////////////////////////////////////////////////////////
void Framebuffer::SetShaderVariables( int shaderID )
{
	glm::vec3 CamPosition = Camera::getInstance().getCameraPosition();
	glUniform3fv(glGetUniformLocation(shaderID, "camPosition"), 1, glm::value_ptr(CamPosition));

	GLint hAlbedoTex = glGetUniformLocation(shaderID, "albedoTexture");
	GLint hPositionTex = glGetUniformLocation(shaderID, "positionTexture");
	GLint hNormalTex = glGetUniformLocation(shaderID, "normalTexture");
	GLint hCubemapTex = glGetUniformLocation(shaderID, "cubemapTexture");

	glUniform1i(hPositionTex, 0);
	glUniform1i(hNormalTex, 1);
	glUniform1i(hAlbedoTex, 2);
	glUniform1i(hCubemapTex, 3);

	// Set Directional light related shader variables...
	DirectionalLightIlluminance(shaderID);

	// Set Point light related shader variables...
	PointLightIlluminance(shaderID);
}

//////////////////////////////////////////////////////////////////////////
void Framebuffer::PointLightIlluminance( int shaderID )
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

//////////////////////////////////////////////////////////////////////////
void Framebuffer::DirectionalLightIlluminance( int shaderID )
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

