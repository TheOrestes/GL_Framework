
#pragma once

#include "GL\glew.h"
#include "../Helpers/VertexStructures.h"

class GLSLShader;

class Framebuffer
{
public:
	Framebuffer();	
	~Framebuffer();

	void			FramebufferSetup();
	void			BeginRenderToFramebuffer();
	void			EndRenderToFramebuffer();
	void			RenderFramebuffer();
	void			RenderBloomEffect();

private:
	void			BlurPass();
	void			BlendPass();

private:
	GLuint			fbo;				// Framebuffer Object
	GLuint			rbo;				// RenderBuffer Object
	GLuint			tbo[2];				// Texture buffer Object, one for normal 
										// post processing & one for brightness threshold

	GLuint			pingpongFBO[2];
	GLuint			pingpongColorBuffer[2];

	GLSLShader*		m_pGenericPostFX;
	GLSLShader*		m_pBlurPostFX;
	GLSLShader*		m_pBloomPostFX;

	// Screen aligned Quad
	GLuint			vbo;
	GLuint			vao;
	GLint			posAttrib;
	GLint			texAttrib;
	VertexPT		quadVertices[6]; 

	bool			m_bBloomOn;
	bool	        horizontal;
};