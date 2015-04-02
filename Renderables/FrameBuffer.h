
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
	

private:
	GLuint			fbo;				// Framebuffer Object
	GLuint			rbo;				// RenderBuffer Object
	GLuint			tbo;				// Texture buffer Object

	GLSLShader*		m_pShader;

	// Screen aligned Quad
	GLuint			vbo;
	GLuint			vao;
	GLint			posAttrib;
	GLint			texAttrib;
	VertexPT		quadVertices[6]; 
};