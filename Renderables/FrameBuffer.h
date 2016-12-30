
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
		m_iBloomSamples = 5;
	}

	PostFXData(bool flag, float exp, int samples, float threshold, float xRes, float yRes)
	{
		m_bBloomOn = flag;
		m_fExposure = exp;
		m_iBloomSamples = samples;
		m_fBloomThreshold = threshold;
		m_vecResolution = glm::vec2(xRes, yRes);
	}

	bool		m_bBloomOn;
	float		m_fExposure;
	int			m_iBloomSamples;
	float		m_fBloomThreshold;
	glm::vec2	m_vecResolution;
};

class Framebuffer
{
public:
	Framebuffer();	
	~Framebuffer();

	void			FramebufferSetup();
	void			BeginRenderGeometryPass();
	void			EndRenderGeometryPass();
	void			RenderDeferredLightingPass();
	void			RenderPostProcessingPass();
	void			RenderBloomEffectPass();
	void			SetShaderVariables(int shaderID);

private:
	void			BlurPass();
	void			BlendPass();
	void			PointLightIlluminance(int shaderID);
	void			DirectionalLightIlluminance(int shaderID);			

private:
	GLuint			fbo;					// Framebuffer Object
	GLuint			rbo;					// RenderBuffer Object
	GLuint			tbo[MAX_NUM_BUFFER];	// Texture buffer Objects, for deferred rendering!

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

	// Render deferred pass to framebuffer object (Pre-Bloom pass) 
	GLuint			fboDeferred;
	GLuint			rboDeferred;
	GLuint			tboDeferred[2];			// for deferred data & brightness data

	// Bloom Pass
	GLuint			fboPingPong[2];
	GLuint			tboPingPong[2];			

	// Tweakable data
	PostFXData*		m_pFXData;
	TwBar*			m_pFXUI;

	void			GeometryPassFrameBufferSetup();
	void			DeferredPassFrameBufferSetup();
	void			BloomPassFrameBufferSetup();
};