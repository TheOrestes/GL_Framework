
#pragma once

#include "glew.h"
#include "../Helpers/VertexStructures.h"
#include "AntTweakBar.h"

class GLSLShader;

enum FramebufferType
{
	POSITION_BUFFER = 0,
	NORMAL_BUFFER,
	ALBEDO_COLOR_BUFFER,
	CUBEMAP_BUFFER,
	EMISSIVE_BUFFER,
	MAX_NUM_BUFFER
};

struct PostFXData
{
	PostFXData()
	{
		m_bBloomOn = true;
		m_fExposure = 1.2f;
		m_iBlurIter = 5;
	}

	bool	m_bBloomOn;
	float	m_fExposure;
	int		m_iBlurIter;
};

class Framebuffer
{
public:
	Framebuffer();	
	~Framebuffer();

	void			FramebufferSetup();
	void			BeginRenderToFramebuffer();
	void			EndRenderToFramebuffer();
	void			RenderDeferredLightingPass();
	void			RenderBloomEffect();
	void			SetShaderVariables(int shaderID);

private:
	void			BlurPass();
	void			BlendPass();
	void			PointLightIlluminance(int shaderID);
	void			DirectionalLightIlluminance(int shaderID);			

private:
	GLuint			fbo;					// Framebuffer Object
	GLuint			rbo;					// RenderBuffer Object
	GLuint			tbo[MAX_NUM_BUFFER+1];	// Texture buffer Object, one for normal 
											// post processing & one for brightness threshold

	GLuint			bloomFBO[2];
	GLuint			bloomColorBuffer[2];

	GLSLShader*		m_pGenericPostFX;
	GLSLShader*		m_pBlurPostFX;
	GLSLShader*		m_pBloomPostFX;
	GLSLShader*		m_pDeferredLightingFX;

	// Screen aligned Quad
	GLuint			vbo;
	GLuint			vao;
	GLint			posAttrib;
	GLint			texAttrib;
	VertexPT		quadVertices[6]; 
	bool	        horizontal;

	// Tweakable data
	PostFXData*		m_pFXData;
	TwBar*			m_pFXUI;
};