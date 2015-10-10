
#include "GL/glew.h"
#include "glfw3.h"
#include "OculusPlayer.h"
#include "../Camera/Camera.h"

//////////////////////////////////////////////////////////////////////////////////////////
OculusPlayer::OculusPlayer()
{
	ovr_fbo_le = -1;
	ovr_tbo_le = -1;
	ovr_dbo_le = -1;

	ovr_fbo_le = -1;
	ovr_tbo_le = -1;
	ovr_dbo_le = -1;

	ovr_textureSet_le = nullptr;
	ovr_textureSet_re = nullptr;

	ovr_eyeHeight = OVR::Vector3f(0.0f,1.6f,-5.0f);
}

//////////////////////////////////////////////////////////////////////////////////////////
OculusPlayer::~OculusPlayer()
{
	KillOculus();
}

//////////////////////////////////////////////////////////////////////////////////////////
bool OculusPlayer::InitializeOculus(float w, float h)
{
	ovrGraphicsLuid luid;

	// Initialize LibOVR...
	OVR::System::Init();
	ovrResult result = ovr_Initialize(nullptr);
	if(OVR_FAILURE(result))
	{
		printf("\novr_Initialize() FAILED!");
		return false;
	}

	result = ovr_Create(&ovr_HMD, &luid);
	if(OVR_FAILURE(result))
	{
		printf("\novr_Create() FAILED!");
		return false;
	}

	// Get device description...
	ovr_hmdDesc = ovr_GetHmdDesc(ovr_HMD);
	printf("\nRift with Serial Number : %s connected!", ovr_hmdDesc.SerialNumber);

	// Decide on window size for Oculus...
	int div = 1;
	ovrSizei windowSize = { ovr_hmdDesc.Resolution.w / 2, ovr_hmdDesc.Resolution.h / 2};

	// Start sensor input ...
	result = ovr_ConfigureTracking(ovr_HMD, ovrTrackingCap_Orientation | 
										ovrTrackingCap_MagYawCorrection | 
										ovrTrackingCap_Position, 0);

	if(OVR_FAILURE(result))
	{
		printf("\novr_ConfigureTracking() FAILED!");
		return false;
	}

	// Init LE buffer
	ovr_textureSize_le = ovr_GetFovTextureSize(ovr_HMD, ovrEye_Left, 
											   ovr_hmdDesc.DefaultEyeFov[ovrEye_Left], 1);
	result = ovr_CreateSwapTextureSetGL(ovr_HMD, GL_SRGB8_ALPHA8, 
										ovr_textureSize_le.w, ovr_textureSize_le.h,
										&ovr_textureSet_le);

	if(OVR_FAILURE(result))
	{
		printf("\ovr_CreateSwapTextureSetGL() FAILED!");
		return false;
	}

	if (OVR_SUCCESS(result))
	{
		for (unsigned int i = 0 ; i < ovr_textureSet_le->TextureCount ; ++i)
		{
			ovrGLTexture* texLE = (ovrGLTexture*)&ovr_textureSet_le->Textures[i];

			glBindTexture(GL_TEXTURE_2D, texLE->OGL.TexId);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		}

		glGenFramebuffers(1, &ovr_fbo_le);

		glGenTextures(1, &ovr_dbo_le);
		glBindTexture(GL_TEXTURE_2D, ovr_dbo_le);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT24, 
					 ovr_textureSize_le.w, ovr_textureSize_le.h, 0, GL_DEPTH_COMPONENT, 
					 GL_UNSIGNED_INT, NULL);
	}

	// Init RE buffer
	ovr_textureSize_re = ovr_GetFovTextureSize(ovr_HMD, ovrEye_Right, 
											   ovr_hmdDesc.DefaultEyeFov[ovrEye_Right], 1);
	result = ovr_CreateSwapTextureSetGL(ovr_HMD, GL_SRGB8_ALPHA8, 
										ovr_textureSize_re.w, ovr_textureSize_re.h,
										&ovr_textureSet_re);

	if(OVR_FAILURE(result))
	{
		printf("\ovr_CreateSwapTextureSetGL() FAILED!");
		return false;
	}

	if (OVR_SUCCESS(result))
	{
		for (unsigned int i = 0 ; i < ovr_textureSet_re->TextureCount ; ++i)
		{
			ovrGLTexture* texRE = (ovrGLTexture*)&ovr_textureSet_re->Textures[i];

			glBindTexture(GL_TEXTURE_2D, texRE->OGL.TexId);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		}

		glGenFramebuffers(1, &ovr_fbo_re);

		glGenTextures(1, &ovr_dbo_re);
		glBindTexture(GL_TEXTURE_2D, ovr_dbo_re);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT24, 
					 ovr_textureSize_re.w, ovr_textureSize_re.h, 0, GL_DEPTH_COMPONENT, 
					 GL_UNSIGNED_INT, NULL);
	}

	// Mirror texture & fbo, used to copy mirror texture to backbuffer...
	result = ovr_CreateMirrorTextureGL(ovr_HMD, GL_SRGB8_ALPHA8, windowSize.w, windowSize.h,
									   reinterpret_cast<ovrTexture**>(&ovr_textureMirror));

	if(OVR_FAILURE(result))
	{
		printf("\ovr_CreateMirrorTextureGL() FAILED!");
		return false;
	}

	glGenFramebuffers(1, &ovr_fboMirror);
	glBindFramebuffer(GL_READ_FRAMEBUFFER, ovr_fboMirror);
	glFramebufferTexture2D(GL_READ_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, 
						   ovr_textureMirror->OGL.TexId, 0);
	glFramebufferRenderbuffer(GL_READ_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, 0);
	glBindFramebuffer(GL_READ_FRAMEBUFFER, 0);

	// Get Render description for both the Eyes...
	ovr_renderDesc_le = ovr_GetRenderDesc(ovr_HMD, ovrEye_Left, ovr_hmdDesc.DefaultEyeFov[0]);
	ovr_renderDesc_re = ovr_GetRenderDesc(ovr_HMD, ovrEye_Right, ovr_hmdDesc.DefaultEyeFov[1]);

	return true;
}

//////////////////////////////////////////////////////////////////////////////////////////
void OculusPlayer::UpdateOculusData()
{
	// Get player height...
	ovr_yaw = 3.141592f; 
	ovr_eyeHeight.y = ovr_GetFloat(ovr_HMD, OVR_KEY_EYE_HEIGHT, 1.6f);

	// Get eye poses, correct IPD offsets
	ovr_viewOffset[0] = ovr_renderDesc_le.HmdToEyeViewOffset;
	ovr_viewOffset[1] = ovr_renderDesc_re.HmdToEyeViewOffset;

	ovrFrameTiming fTiming  = ovr_GetFrameTiming(ovr_HMD, 0);
	ovrTrackingState hmdState = ovr_GetTrackingState(ovr_HMD, fTiming.DisplayMidpointSeconds);
	ovr_CalcEyePoses(hmdState.HeadPose.ThePose, ovr_viewOffset, ovr_eyeRenderPos);
}

//////////////////////////////////////////////////////////////////////////////////////////
void OculusPlayer::BeginRenderOculusLE()
{ 
	ovr_textureSet_le->CurrentIndex = (ovr_textureSet_le->CurrentIndex + 1) % ovr_textureSet_le->TextureCount;
	ovrGLTexture* texLE = reinterpret_cast<ovrGLTexture*>(&ovr_textureSet_le->Textures[ovr_textureSet_le->CurrentIndex]);

	glBindFramebuffer(GL_FRAMEBUFFER, ovr_fbo_le);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texLE->OGL.TexId, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, ovr_dbo_le, 0);

	glViewport(0, 0, ovr_textureSize_le.w, ovr_textureSize_le.h);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_FRAMEBUFFER_SRGB);

	// Get view and projection matrices
	OVR::Matrix4f rollPitchYawLE = OVR::Matrix4f::RotationY(ovr_yaw);
	OVR::Matrix4f finalRollPitchYawLE = rollPitchYawLE * OVR::Matrix4f(ovr_eyeRenderPos[ovrEye_Left].Orientation);
	OVR::Vector3f finalUpLE = finalRollPitchYawLE.Transform(OVR::Vector3f(0, 1, 0));
	OVR::Vector3f finalForwardLE = finalRollPitchYawLE.Transform(OVR::Vector3f(0, 0, -1));
	OVR::Vector3f shiftedEyePosLE = ovr_eyeHeight + rollPitchYawLE.Transform(ovr_eyeRenderPos[ovrEye_Left].Position);
	OVR::Matrix4f viewLE = OVR::Matrix4f::LookAtRH(shiftedEyePosLE, shiftedEyePosLE + finalForwardLE, finalUpLE);
	OVR::Matrix4f projLE = ovrMatrix4f_Projection(ovr_hmdDesc.DefaultEyeFov[ovrEye_Left], 0.2f, 1000.0f, ovrProjection_RightHanded);

	//***** OVR values to engine matrices value transfer *****//
	glm::vec3 _posLE(shiftedEyePosLE.x, shiftedEyePosLE.y, shiftedEyePosLE.z);

	glm::mat4 _viewLE(viewLE.M[0][0], viewLE.M[0][1], viewLE.M[0][2], viewLE.M[0][3],
					  viewLE.M[1][0], viewLE.M[1][1], viewLE.M[1][2], viewLE.M[1][3],
					  viewLE.M[2][0], viewLE.M[2][1], viewLE.M[2][2], viewLE.M[2][3],
					  viewLE.M[3][0], viewLE.M[3][1], viewLE.M[3][2], viewLE.M[3][3]);

	glm::mat4 _projLE(projLE.M[0][0], projLE.M[0][1], projLE.M[0][2], projLE.M[0][3],
					  projLE.M[1][0], projLE.M[1][1], projLE.M[1][2], projLE.M[1][3],
					  projLE.M[2][0], projLE.M[2][1], projLE.M[2][2], projLE.M[2][3],
					  projLE.M[3][0], projLE.M[3][1], projLE.M[3][2], projLE.M[3][3]);

	Camera::getInstance().setPosition(_posLE);
	Camera::getInstance().setViewMatrix(_viewLE);
	Camera::getInstance().setProjMatrix(_projLE);
}

//////////////////////////////////////////////////////////////////////////////////////////
void OculusPlayer::EndRenderOculusLE()
{
	glBindFramebuffer(GL_FRAMEBUFFER, ovr_fbo_le);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, 0, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, 0, 0);
}

//////////////////////////////////////////////////////////////////////////////////////////
void OculusPlayer::BeginRenderOculusRE()
{
	ovr_textureSet_re->CurrentIndex = (ovr_textureSet_re->CurrentIndex + 1) % ovr_textureSet_re->TextureCount;
	ovrGLTexture* texRE = reinterpret_cast<ovrGLTexture*>(&ovr_textureSet_re->Textures[ovr_textureSet_re->CurrentIndex]);

	glBindFramebuffer(GL_FRAMEBUFFER, ovr_fbo_re);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texRE->OGL.TexId, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, ovr_dbo_re, 0);

	glViewport(0, 0, ovr_textureSize_re.w, ovr_textureSize_re.h);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_FRAMEBUFFER_SRGB);

	// Get view and projection matrices
	OVR::Matrix4f rollPitchYawRE = OVR::Matrix4f::RotationY(ovr_yaw);
	OVR::Matrix4f finalRollPitchYawRE = rollPitchYawRE * OVR::Matrix4f(ovr_eyeRenderPos[ovrEye_Right].Orientation);
	OVR::Vector3f finalUpRE = finalRollPitchYawRE.Transform(OVR::Vector3f(0, 1, 0));
	OVR::Vector3f finalForwardRE = finalRollPitchYawRE.Transform(OVR::Vector3f(0, 0, -1));
	OVR::Vector3f shiftedEyePosRE = ovr_eyeHeight + rollPitchYawRE.Transform(ovr_eyeRenderPos[ovrEye_Right].Position);
	OVR::Matrix4f viewRE = OVR::Matrix4f::LookAtRH(shiftedEyePosRE, shiftedEyePosRE + finalForwardRE, finalUpRE);
	OVR::Matrix4f projRE = ovrMatrix4f_Projection(ovr_hmdDesc.DefaultEyeFov[ovrEye_Right], 0.2f, 1000.0f, ovrProjection_RightHanded);

	//***** OVR values to engine matrices value transfer *****//
	glm::vec3 _posRE(shiftedEyePosRE.x, shiftedEyePosRE.y, shiftedEyePosRE.z);

	glm::mat4 _viewRE(viewRE.M[0][0], viewRE.M[0][1], viewRE.M[0][2], viewRE.M[0][3],
		viewRE.M[1][0], viewRE.M[1][1], viewRE.M[1][2], viewRE.M[1][3],
		viewRE.M[2][0], viewRE.M[2][1], viewRE.M[2][2], viewRE.M[2][3],
		viewRE.M[3][0], viewRE.M[3][1], viewRE.M[3][2], viewRE.M[3][3]);

	glm::mat4 _projRE(projRE.M[0][0], projRE.M[0][1], projRE.M[0][2], projRE.M[0][3],
		projRE.M[1][0], projRE.M[1][1], projRE.M[1][2], projRE.M[1][3],
		projRE.M[2][0], projRE.M[2][1], projRE.M[2][2], projRE.M[2][3],
		projRE.M[3][0], projRE.M[3][1], projRE.M[3][2], projRE.M[3][3]);

	Camera::getInstance().setPosition(_posRE);
	Camera::getInstance().setViewMatrix(_viewRE);
	Camera::getInstance().setProjMatrix(_projRE);
}

//////////////////////////////////////////////////////////////////////////////////////////
void OculusPlayer::EndRenderOculusRE()
{
	glBindFramebuffer(GL_FRAMEBUFFER, ovr_fbo_re);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, 0, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, 0, 0);
}

//////////////////////////////////////////////////////////////////////////////////////////
void OculusPlayer::FinalizeRenderOculus()
{
	//------------------------------ Distortion Rendering ------------------------------//
	ovrViewScaleDesc viewScaleDesc;
	viewScaleDesc.HmdSpaceToWorldScaleInMeters = 1.0f;
	viewScaleDesc.HmdToEyeViewOffset[0] = ovr_viewOffset[0];
	viewScaleDesc.HmdToEyeViewOffset[1] = ovr_viewOffset[1];

	ovrLayerEyeFov ld;
	ld.Header.Type = ovrLayerType_EyeFov;
	ld.Header.Flags = ovrLayerFlag_TextureOriginAtBottomLeft;

	// LE
	ld.ColorTexture[ovrEye_Left] = ovr_textureSet_le;
	ld.Viewport[ovrEye_Left] = OVR::Recti(ovr_textureSize_le);
	ld.Fov[ovrEye_Left] = ovr_hmdDesc.DefaultEyeFov[ovrEye_Left];
	ld.RenderPose[ovrEye_Left] = ovr_eyeRenderPos[ovrEye_Left];

	// RE
	ld.ColorTexture[ovrEye_Right] = ovr_textureSet_re;
	ld.Viewport[ovrEye_Right] = OVR::Recti(ovr_textureSize_re);
	ld.Fov[ovrEye_Right] = ovr_hmdDesc.DefaultEyeFov[ovrEye_Right];
	ld.RenderPose[ovrEye_Right] = ovr_eyeRenderPos[ovrEye_Right];

	ovrLayerHeader* layers = &ld.Header;
	ovrResult result = ovr_SubmitFrame(ovr_HMD, 0, &viewScaleDesc, &layers, 1);

	// Blit mirror texture to back buffer
	glBindFramebuffer(GL_READ_FRAMEBUFFER, ovr_fboMirror);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
	GLint w = ovr_textureMirror->OGL.Header.TextureSize.w;
	GLint h = ovr_textureMirror->OGL.Header.TextureSize.h;
	glBlitFramebuffer(0, h, w, 0,
					  0, 0, w, h,
					  GL_COLOR_BUFFER_BIT, GL_NEAREST);
	glBindFramebuffer(GL_READ_FRAMEBUFFER, 0);
}

//////////////////////////////////////////////////////////////////////////////////////////
void OculusPlayer::KillOculus()
{
	if(ovr_fboMirror) glDeleteFramebuffers(1, &ovr_fboMirror);
	if(ovr_textureMirror) ovr_DestroyMirrorTexture(ovr_HMD, reinterpret_cast<ovrTexture*>(ovr_textureMirror));

	// LE
	if(ovr_textureSet_le)
	{
		ovr_DestroySwapTextureSet(ovr_HMD, ovr_textureSet_le);
		ovr_textureSet_le = nullptr;
	}

	if (ovr_tbo_le)
	{
		glDeleteTextures(1, &ovr_tbo_le);
		ovr_tbo_le = 0;
	}

	if(ovr_fbo_le)
	{
		glDeleteFramebuffers(1, &ovr_fbo_le);
		ovr_fbo_le = 0;
	}

	// RE
	if(ovr_textureSet_re)
	{
		ovr_DestroySwapTextureSet(ovr_HMD, ovr_textureSet_re);
		ovr_textureSet_re = nullptr;
	}

	if (ovr_tbo_re)
	{
		glDeleteTextures(1, &ovr_tbo_re);
		ovr_tbo_re = 0;
	}

	if(ovr_fbo_re)
	{
		glDeleteFramebuffers(1, &ovr_fbo_re);
		ovr_fbo_re = 0;
	}

	ovr_Destroy(ovr_HMD);
	ovr_Shutdown();
	OVR::System::Destroy();
}