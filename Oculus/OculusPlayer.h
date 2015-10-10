
#pragma once

#include "Kernel/OVR_System.h"
#include "OVR_CAPI_GL.h"
#include "Extras/OVR_Math.h"

class OculusPlayer
{
public:
	OculusPlayer();
	~OculusPlayer();

	bool				InitializeOculus(float w, float h);
	void				BeginRenderOculusLE();
	void				EndRenderOculusLE();
	void				BeginRenderOculusRE();
	void				EndRenderOculusRE();
	void				FinalizeRenderOculus();
	void				UpdateOculusData();
	void				KillOculus();

private:
	// Left Eye specific 
	GLuint				ovr_fbo_le;
	GLuint				ovr_tbo_le;
	GLuint				ovr_dbo_le;
	ovrSizei			ovr_textureSize_le;
	ovrEyeRenderDesc	ovr_renderDesc_le;
	ovrSwapTextureSet*	ovr_textureSet_le;

	// Right eye specific 
	GLuint				ovr_fbo_re;
	GLuint				ovr_tbo_re;
	GLuint				ovr_dbo_re;
	ovrSizei			ovr_textureSize_re;
	ovrEyeRenderDesc	ovr_renderDesc_re;
	ovrSwapTextureSet*	ovr_textureSet_re;

	// HMD related 
	ovrHmd				ovr_HMD;
	ovrHmdDesc			ovr_hmdDesc;
	float				ovr_yaw;
	OVR::Vector3f		ovr_eyeHeight;
	ovrPosef			ovr_eyeRenderPos[2];
	ovrVector3f			ovr_viewOffset[2];
	
	// Final rendering related
	GLuint				ovr_fboMirror;
	ovrGLTexture*		ovr_textureMirror;
};