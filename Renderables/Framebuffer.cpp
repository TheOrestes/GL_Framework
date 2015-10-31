
#include "FrameBuffer.h"
#include "../ShaderEngine/GLSLShader.h"
#include "../Helpers/VertexStructures.h"
#include "../UI/UIManager.h"
#include <iostream>

//////////////////////////////////////////////////////////////////////////////////////////
Framebuffer::Framebuffer()
{
	m_pGenericPostFX = nullptr;
	m_pBlurPostFX = nullptr;
	m_pBloomPostFX = nullptr;

	// Screen aligned Quad setup
	quadVertices[0] = VertexPT(glm::vec3(-1,1,0), glm::vec2(0,1));
	quadVertices[1] = VertexPT(glm::vec3(-1,-1,0), glm::vec2(0,0));
	quadVertices[2] = VertexPT(glm::vec3(1,-1,0), glm::vec2(1,0));
	quadVertices[3] = VertexPT(glm::vec3(-1,1,0), glm::vec2(0,1));
	quadVertices[4] = VertexPT(glm::vec3(1,-1,0), glm::vec2(1,0));
	quadVertices[5] = VertexPT(glm::vec3(1,1,0), glm::vec2(1,1));

	// FX Data
	m_pFXData = nullptr;
}

//////////////////////////////////////////////////////////////////////////////////////////
Framebuffer::~Framebuffer()
{
	delete m_pGenericPostFX;
	delete m_pBlurPostFX;
	delete m_pBloomPostFX;
	delete m_pFXData;
}

//////////////////////////////////////////////////////////////////////////////////////////
void Framebuffer::FramebufferSetup()
{
	// create actual framebuffer object & bind it. 
	glGenFramebuffers(1, &fbo);
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);

		// create two texture buffer objects 
		// 1. Normal Rendering
		// 2. Brightness threshold values
		glGenTextures(2, tbo);
		for (int i = 0 ; i < 2 ; i++)
		{
			glBindTexture(GL_TEXTURE_2D, tbo[i]);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, 1280, 800, 0, GL_RGB, GL_FLOAT, NULL);
			//glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 1280, 800, 0, GL_RGB, GL_UNSIGNED_INT, NULL);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); // We clamp to the edge as the blur filter would otherwise sample repeated texture values!
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
		GLuint attachments[2] = {GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1};
		glDrawBuffers(2, attachments);

		// Finally, check if framebuffer is complete
		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		{
			std::cout << "Frame Buffer status error!" << std::endl;
			return;
		}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

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

	// Load all required shaders for post processing...
	m_pGenericPostFX = new GLSLShader("Shaders/vsFramebuffer.glsl", "Shaders/psFramebuffer.glsl");
	m_pBlurPostFX = new GLSLShader("Shaders/vsFramebuffer.glsl", "Shaders/psBlur.glsl");
	m_pBloomPostFX = new GLSLShader("shaders/vsFramebuffer.glsl", "Shaders/psFramebuffer.glsl");

	//-------------------------- Create screen-aligned Quad 
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

		glGenBuffers(1, &vbo);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(GL_ARRAY_BUFFER, 6*sizeof(VertexPT), quadVertices, GL_STATIC_DRAW);

		posAttrib = glGetAttribLocation(m_pGenericPostFX->GetShaderID(), "in_Position");
		glEnableVertexAttribArray(posAttrib);
		glVertexAttribPointer(posAttrib, 3, GL_FLOAT, GL_FALSE, sizeof(VertexPT), (void*)0);

		texAttrib = glGetAttribLocation(m_pGenericPostFX->GetShaderID(), "in_TexCoord");
		glEnableVertexAttribArray(texAttrib);
		glVertexAttribPointer(texAttrib, 2, GL_FLOAT, GL_FALSE, sizeof(VertexPT), (void*)offsetof(VertexPT, uv));

	glBindVertexArray(0);

	// FX Data initialize
	m_pFXData = new PostFXData();
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
void Framebuffer::RenderFramebuffer()
{
	glClearColor(0.01f, 0.01f, 0.01f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	// No need of any depth testing while rendering a single scene aligned quad...
	glDisable(GL_DEPTH_TEST);

	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	m_pGenericPostFX->Use();

	glUniform1f(glGetUniformLocation(m_pGenericPostFX->GetShaderID(), "exposure"), m_pFXData->m_fExposure);

	/*GLuint shader = m_pShader->GetShaderID();
	GLint hTex = glGetUniformLocation(shader, "screenTexture");
	glUniform1i(hTex, 0);*/

	glActiveTexture(GL_TEXTURE0);

	glBindVertexArray(vao);
	glBindTexture(GL_TEXTURE_2D, tbo[0]);

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
	UIManager::getInstance().RenderPostFxUI(m_pFXData);

	// Bloom controlled by UI?
	if(m_pFXData->m_bBloomOn)
	{
		BlurPass();
		BlendPass();
	}
}

