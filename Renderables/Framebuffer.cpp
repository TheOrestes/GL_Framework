
#include "FrameBuffer.h"
#include "../ShaderEngine/GLSLShader.h"
#include "../Helpers/VertexStructures.h"
#include <iostream>

//////////////////////////////////////////////////////////////////////////////////////////
Framebuffer::Framebuffer()
{
	m_pShader = nullptr;

	// Screen aligned Quad setup
	quadVertices[0] = VertexPT(glm::vec3(-1,1,0), glm::vec2(0,1));
	quadVertices[1] = VertexPT(glm::vec3(-1,-1,0), glm::vec2(0,0));
	quadVertices[2] = VertexPT(glm::vec3(1,-1,0), glm::vec2(1,0));
	quadVertices[3] = VertexPT(glm::vec3(-1,1,0), glm::vec2(0,1));
	quadVertices[4] = VertexPT(glm::vec3(1,-1,0), glm::vec2(1,0));
	quadVertices[5] = VertexPT(glm::vec3(1,1,0), glm::vec2(1,1));
}

//////////////////////////////////////////////////////////////////////////////////////////
Framebuffer::~Framebuffer()
{
	delete m_pShader;
}

//////////////////////////////////////////////////////////////////////////////////////////
void Framebuffer::FramebufferSetup()
{
	// create texture buffer object 
	glGenTextures(1, &tbo);
	glBindTexture(GL_TEXTURE_2D, tbo);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, 1280, 800, 0, GL_RGB, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glBindTexture(GL_TEXTURE_2D, 0);


	// create actual framebuffer object & bind it. 
	glGenFramebuffers(1, &fbo);
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);

		
		// attach texture object to already bind framebuffer as color attachment
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, tbo, 0);
		
		// create render buffer object
		glGenRenderbuffers(1, &rbo);
		glBindRenderbuffer(GL_RENDERBUFFER, rbo);
			glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, 1280, 800);
		glBindRenderbuffer(GL_RENDERBUFFER, 0);

		// attach render buffer object as depth & stencil attachment to framebuffer
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);

		// Finally, check if framebuffer is complete
		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		{
			std::cout << "Frame Buffer status error!" << std::endl;
			return;
		}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);


	//-------------------------- Create screen-aligned Quad 
	m_pShader = new GLSLShader("Shaders/vsFramebuffer.glsl", "Shaders/psFramebuffer.glsl");

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

		glGenBuffers(1, &vbo);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(GL_ARRAY_BUFFER, 6*sizeof(VertexPT), quadVertices, GL_STATIC_DRAW);

		posAttrib = glGetAttribLocation(m_pShader->GetShaderID(), "in_Position");
		glEnableVertexAttribArray(posAttrib);
		glVertexAttribPointer(posAttrib, 3, GL_FLOAT, GL_FALSE, sizeof(VertexPT), (void*)0);

		texAttrib = glGetAttribLocation(m_pShader->GetShaderID(), "in_TexCoord");
		glEnableVertexAttribArray(texAttrib);
		glVertexAttribPointer(texAttrib, 2, GL_FLOAT, GL_FALSE, sizeof(VertexPT), (void*)offsetof(VertexPT, uv));

	glBindVertexArray(0);
}

//////////////////////////////////////////////////////////////////////////////////////////
void Framebuffer::BeginRenderToFramebuffer()
{
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);

	glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
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
	glClearColor(1,1,1,1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	// No need of any depth testing while rendering a single scene aligned quad...
	glDisable(GL_DEPTH_TEST);

	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	m_pShader->Use();

	/*GLuint shader = m_pShader->GetShaderID();
	GLint hTex = glGetUniformLocation(shader, "screenTexture");
	glUniform1i(hTex, 0);*/

	glActiveTexture(GL_TEXTURE0);

	glBindVertexArray(vao);
	glBindTexture(GL_TEXTURE_2D, tbo);

	glDrawArrays(GL_TRIANGLES, 0, 6);

	glBindVertexArray(0);
}

