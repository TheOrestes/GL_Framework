
#include <iostream>

#include "GL/glew.h"
#include "glfw3.h"

#include "Camera/Camera.h"
#include "Scene/Scene.h"
#include "Renderables/FrameBuffer.h"

#include "Kernel/OVR_System.h"
#include "OVR_CAPI_GL.h"
#include "Extras/OVR_Math.h"

//////////////////////////////////////////////////////////////////////////////////////////
GLFWwindow* window;
Scene		gScene;
Framebuffer* gFBufferPtr;  

//GLCube		cube;

const float gScreenWidth	=	960.0f;
const float gScreenHeight	=	540.0f;

const float tick			=	0.016667f;
bool  bFirstMouse			=	true;
float lastX					=	gScreenWidth / 2.0f;
float lastY					=	gScreenHeight / 2.0f;

// Oculus
GLuint				fboLE;
GLuint				tboLE;
GLuint				dboLE;
ovrSizei			textureSizeLE;
ovrEyeRenderDesc	renderDescLE;
ovrSwapTextureSet*  TextureSetLE = nullptr;
void				InitBuffersLE();
void				RenderBuffersLE();

GLuint				fboRE;
GLuint				tboRE;
GLuint				dboRE;
ovrSizei			textureSizeRE;
ovrEyeRenderDesc	renderDescRE;
ovrSwapTextureSet*  TextureSetRE = nullptr;
void				InitBuffersRE();
void				RenderBuffersRE();
void				RenderOculus();

ovrHmd				HMD;
ovrHmdDesc			hmdDesc;
GLuint				fboMirror;
ovrGLTexture*		mirrorTexture = nullptr;


//////////////////////////////////////////////////////////////////////////////////////////
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if(key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, true);
	}

	if (key == GLFW_KEY_W && (action == GLFW_REPEAT || GLFW_PRESS))
	{
		Camera::getInstance().ProcessKeyboard(CameraMovement::FORWARD, tick);	
	}

	if (key == GLFW_KEY_S && (action == GLFW_REPEAT || GLFW_PRESS))
	{
		Camera::getInstance().ProcessKeyboard(CameraMovement::BACK, tick);	
	}

	if (key == GLFW_KEY_A && (action == GLFW_REPEAT || GLFW_PRESS))
	{
		Camera::getInstance().ProcessKeyboard(CameraMovement::LEFT, tick);	
	}

	if (key == GLFW_KEY_D && (action == GLFW_REPEAT || GLFW_PRESS))
	{
		Camera::getInstance().ProcessKeyboard(CameraMovement::RIGHT, tick);	
	}
}

//////////////////////////////////////////////////////////////////////////////////////////
void Mouse_Callback(GLFWwindow* window, double xPos, double yPos)
{
	if(bFirstMouse)
	{
		lastX = xPos;
		lastY = yPos;
		bFirstMouse = false;
	}

	GLfloat xoffset = xPos - lastX;
	GLfloat yoffset = lastY - yPos; 
	lastX = xPos;
	lastY = yPos;

	Camera::getInstance().ProcessMouseMovement(xoffset, yoffset);
}

//////////////////////////////////////////////////////////////////////////////////////////
void GameLoop(float tick)
{
	/*glfwPollEvents();

	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);*/

	RenderOculus();
}

//////////////////////////////////////////////////////////////////////////////////////////
void InitializeOculus()
{
	ovrGraphicsLuid luid;

	OVR::System::Init();
	ovrResult result = ovr_Initialize(nullptr);
	if(OVR_FAILURE(result))
		printf("\novr_Initialize() FAILED!");

	result = ovr_Create(&HMD, &luid);
	if(OVR_FAILURE(result))
		printf("\novr_Create() FAILED!");

	hmdDesc = ovr_GetHmdDesc(HMD);

	printf("\nRift Serial Number : %s", hmdDesc.SerialNumber);
	printf("\nRift Manufacturer : %s", hmdDesc.Manufacturer);

	ovrSizei windowSize = { hmdDesc.Resolution.w / 2, hmdDesc.Resolution.h / 2};

	result = ovr_ConfigureTracking(HMD, ovrTrackingCap_Orientation | 
										ovrTrackingCap_MagYawCorrection | 
										ovrTrackingCap_Position, 0);

	// Init LE buffers
	{
		textureSizeLE = ovr_GetFovTextureSize(HMD, ovrEye_Left, 
													   hmdDesc.DefaultEyeFov[ovrEye_Left],
													   1);
		result = ovr_CreateSwapTextureSetGL(HMD, GL_SRGB8_ALPHA8, 
											textureSizeLE.w, textureSizeLE.h,
											&TextureSetLE);

		if (OVR_SUCCESS(result))
		{
			for (unsigned int i = 0 ; i < TextureSetLE->TextureCount ; ++i)
			{
				ovrGLTexture* texLE = (ovrGLTexture*)&TextureSetLE->Textures[i];

				glBindTexture(GL_TEXTURE_2D, texLE->OGL.TexId);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			}
			
			glGenFramebuffers(1, &fboLE);

			glGenTextures(1, &dboLE);
			glBindTexture(GL_TEXTURE_2D, dboLE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT24, 
						 textureSizeLE.w, textureSizeLE.h, 0, GL_DEPTH_COMPONENT, 
						 GL_UNSIGNED_INT, NULL);
		}
	}

	// Init RE buffers
	{
		textureSizeRE = ovr_GetFovTextureSize(HMD, ovrEye_Right, 
													   hmdDesc.DefaultEyeFov[ovrEye_Left],
													   1);
		result = ovr_CreateSwapTextureSetGL(HMD, GL_SRGB8_ALPHA8, 
											textureSizeRE.w, textureSizeRE.h,
											&TextureSetRE);

		if (OVR_SUCCESS(result))
		{
			for (unsigned int i = 0 ; i < TextureSetRE->TextureCount ; ++i)
			{
				ovrGLTexture* texRE = (ovrGLTexture*)&TextureSetRE->Textures[i];

				glBindTexture(GL_TEXTURE_2D, texRE->OGL.TexId);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			}

			glGenFramebuffers(1, &fboRE);

			glGenTextures(1, &dboRE);
			glBindTexture(GL_TEXTURE_2D, dboRE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT24, 
				textureSizeRE.w, textureSizeRE.h, 0, GL_DEPTH_COMPONENT, 
				GL_UNSIGNED_INT, NULL);
		}
	}

	// Mirror texture & fbo, used to copy mirror texture to backbuffer...
	result = ovr_CreateMirrorTextureGL(HMD, GL_SRGB8_ALPHA8, windowSize.w, windowSize.h,
									   reinterpret_cast<ovrTexture**>(&mirrorTexture));

	glGenFramebuffers(1, &fboMirror);
	glBindFramebuffer(GL_READ_FRAMEBUFFER, fboMirror);
	glFramebufferTexture2D(GL_READ_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, 
						   mirrorTexture->OGL.TexId, 0);
	glFramebufferRenderbuffer(GL_READ_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, 0);
	glBindFramebuffer(GL_READ_FRAMEBUFFER, 0);

	// Get Render description for both the Eyes...
	renderDescLE = ovr_GetRenderDesc(HMD, ovrEye_Left, hmdDesc.DefaultEyeFov[0]);
	renderDescRE = ovr_GetRenderDesc(HMD, ovrEye_Right, hmdDesc.DefaultEyeFov[1]);
}

//////////////////////////////////////////////////////////////////////////////////////////
void RenderOculus()
{
	// Get player height...
	float Yaw = 3.141592f; 
	OVR::Vector3f Pos2(0.0f,1.6f,-5.0f);

	Pos2.y = ovr_GetFloat(HMD, OVR_KEY_EYE_HEIGHT, 1.6f);

	// Get eye poses, correct IPD offsets
	ovrVector3f viewOffset[2] = { renderDescLE.HmdToEyeViewOffset, renderDescRE.HmdToEyeViewOffset };

	ovrPosef EyeRenderPose[2];

	ovrFrameTiming fTiming  = ovr_GetFrameTiming(HMD, 0);
	ovrTrackingState hmdState = ovr_GetTrackingState(HMD, fTiming.DisplayMidpointSeconds);
	ovr_CalcEyePoses(hmdState.HeadPose.ThePose, viewOffset, EyeRenderPose);

	//------------------------------------ Left Eye ------------------------------------// 
	TextureSetLE->CurrentIndex = (TextureSetLE->CurrentIndex + 1) % TextureSetLE->TextureCount;
	ovrGLTexture* texLE = reinterpret_cast<ovrGLTexture*>(&TextureSetLE->Textures[TextureSetLE->CurrentIndex]);

	glBindFramebuffer(GL_FRAMEBUFFER, fboLE);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texLE->OGL.TexId, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, dboLE, 0);

	glViewport(0, 0, textureSizeLE.w, textureSizeLE.h);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_FRAMEBUFFER_SRGB);

	// Get view and projection matrices
	OVR::Matrix4f rollPitchYawLE = OVR::Matrix4f::RotationY(Yaw);
	OVR::Matrix4f finalRollPitchYawLE = rollPitchYawLE * OVR::Matrix4f(EyeRenderPose[ovrEye_Left].Orientation);
	OVR::Vector3f finalUpLE = finalRollPitchYawLE.Transform(OVR::Vector3f(0, 1, 0));
	OVR::Vector3f finalForwardLE = finalRollPitchYawLE.Transform(OVR::Vector3f(0, 0, -1));
	OVR::Vector3f shiftedEyePosLE = Pos2 + rollPitchYawLE.Transform(EyeRenderPose[ovrEye_Left].Position);
	OVR::Matrix4f viewLE = OVR::Matrix4f::LookAtRH(shiftedEyePosLE, shiftedEyePosLE + finalForwardLE, finalUpLE);
	OVR::Matrix4f projLE = ovrMatrix4f_Projection(hmdDesc.DefaultEyeFov[ovrEye_Left], 0.2f, 1000.0f, ovrProjection_RightHanded);

	glm::vec3 _posLE(shiftedEyePosLE.x, shiftedEyePosLE.y, shiftedEyePosLE.z);

	glm::mat4 _viewLE(viewLE.M[0][0], viewLE.M[0][1], viewLE.M[0][2], viewLE.M[0][3],
		viewLE.M[1][0], viewLE.M[1][1], viewLE.M[1][2], viewLE.M[1][3],
		viewLE.M[2][0], viewLE.M[2][1], viewLE.M[2][2], viewLE.M[2][3],
		viewLE.M[3][0], viewLE.M[3][1], viewLE.M[3][2], viewLE.M[3][3]);

	glm::mat4 _projLE(projLE.M[0][0], projLE.M[0][1], projLE.M[0][2], projLE.M[0][3],
		projLE.M[1][0], projLE.M[1][1], projLE.M[1][2], projLE.M[1][3],
		projLE.M[2][0], projLE.M[2][1], projLE.M[2][2], projLE.M[2][3],
		projLE.M[3][0], projLE.M[3][1], projLE.M[3][2], projLE.M[3][3]);

	// Set View & Projection matrix for normal scene rendering...
	Camera::getInstance().setPosition(_posLE);
	Camera::getInstance().setViewMatrix(_viewLE);
	Camera::getInstance().setProjMatrix(_projLE);

		// Render scene !!!
		gScene.Update(tick);
		gScene.Render();


	glBindFramebuffer(GL_FRAMEBUFFER, fboLE);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, 0, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, 0, 0);

	//----------------------------------- Right Eye ------------------------------------//
	TextureSetRE->CurrentIndex = (TextureSetRE->CurrentIndex + 1) % TextureSetRE->TextureCount;
	ovrGLTexture* texRE = reinterpret_cast<ovrGLTexture*>(&TextureSetRE->Textures[TextureSetRE->CurrentIndex]);

	glBindFramebuffer(GL_FRAMEBUFFER, fboRE);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texRE->OGL.TexId, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, dboRE, 0);

	glViewport(0, 0, textureSizeRE.w, textureSizeRE.h);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_FRAMEBUFFER_SRGB);

	// Get view and projection matrices
	OVR::Matrix4f rollPitchYawRE = OVR::Matrix4f::RotationY(Yaw);
	OVR::Matrix4f finalRollPitchYawRE = rollPitchYawRE * OVR::Matrix4f(EyeRenderPose[ovrEye_Right].Orientation);
	OVR::Vector3f finalUpRE = finalRollPitchYawRE.Transform(OVR::Vector3f(0, 1, 0));
	OVR::Vector3f finalForwardRE = finalRollPitchYawRE.Transform(OVR::Vector3f(0, 0, -1));
	OVR::Vector3f shiftedEyePosRE = Pos2 + rollPitchYawRE.Transform(EyeRenderPose[ovrEye_Right].Position);
	OVR::Matrix4f viewRE = OVR::Matrix4f::LookAtRH(shiftedEyePosRE, shiftedEyePosRE + finalForwardRE, finalUpRE);
	OVR::Matrix4f projRE = ovrMatrix4f_Projection(hmdDesc.DefaultEyeFov[ovrEye_Right], 0.2f, 1000.0f, ovrProjection_RightHanded);

	glm::vec3 _posRE(shiftedEyePosRE.x, shiftedEyePosRE.y, shiftedEyePosRE.z);

	glm::mat4 _viewRE(viewRE.M[0][0], viewRE.M[0][1], viewRE.M[0][2], viewRE.M[0][3],
		viewRE.M[1][0], viewRE.M[1][1], viewRE.M[1][2], viewRE.M[1][3],
		viewRE.M[2][0], viewRE.M[2][1], viewRE.M[2][2], viewRE.M[2][3],
		viewRE.M[3][0], viewRE.M[3][1], viewRE.M[3][2], viewRE.M[3][3]);

	glm::mat4 _projRE(projRE.M[0][0], projRE.M[0][1], projRE.M[0][2], projRE.M[0][3],
		projRE.M[1][0], projRE.M[1][1], projRE.M[1][2], projRE.M[1][3],
		projRE.M[2][0], projRE.M[2][1], projRE.M[2][2], projRE.M[2][3],
		projRE.M[3][0], projRE.M[3][1], projRE.M[3][2], projRE.M[3][3]);

	// Set View & Projection matrix for normal scene rendering...
	Camera::getInstance().setPosition(_posRE);
	Camera::getInstance().setViewMatrix(_viewRE);
	Camera::getInstance().setProjMatrix(_projRE);

		// Render scene !!!
		gScene.Update(tick);
		gScene.Render();

	glBindFramebuffer(GL_FRAMEBUFFER, fboRE);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, 0, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, 0, 0);

	//------------------------------ Distortion Rendering ------------------------------//
	ovrViewScaleDesc viewScaleDesc;
	viewScaleDesc.HmdSpaceToWorldScaleInMeters = 1.0f;
	viewScaleDesc.HmdToEyeViewOffset[0] = viewOffset[0];
	viewScaleDesc.HmdToEyeViewOffset[1] = viewOffset[1];

	ovrLayerEyeFov ld;
	ld.Header.Type = ovrLayerType_EyeFov;
	ld.Header.Flags = ovrLayerFlag_TextureOriginAtBottomLeft;

	// LE
	ld.ColorTexture[ovrEye_Left] = TextureSetLE;
	ld.Viewport[ovrEye_Left] = OVR::Recti(textureSizeLE);
	ld.Fov[ovrEye_Left] = hmdDesc.DefaultEyeFov[ovrEye_Left];
	ld.RenderPose[ovrEye_Left] = EyeRenderPose[ovrEye_Left];

	// RE
	ld.ColorTexture[ovrEye_Right] = TextureSetRE;
	ld.Viewport[ovrEye_Right] = OVR::Recti(textureSizeRE);
	ld.Fov[ovrEye_Right] = hmdDesc.DefaultEyeFov[ovrEye_Right];
	ld.RenderPose[ovrEye_Right] = EyeRenderPose[ovrEye_Right];

	ovrLayerHeader* layers = &ld.Header;
	ovrResult result = ovr_SubmitFrame(HMD, 0, &viewScaleDesc, &layers, 1);

	// Blit mirror texture to back buffer
	glBindFramebuffer(GL_READ_FRAMEBUFFER, fboMirror);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
	GLint w = mirrorTexture->OGL.Header.TextureSize.w;
	GLint h = mirrorTexture->OGL.Header.TextureSize.h;
	glBlitFramebuffer(0, h, w, 0,
					  0, 0, w, h,
					  GL_COLOR_BUFFER_BIT, GL_NEAREST);
	glBindFramebuffer(GL_READ_FRAMEBUFFER, 0);
}

//////////////////////////////////////////////////////////////////////////////////////////
void KillOculus()
{
	if(fboMirror) glDeleteFramebuffers(1, &fboMirror);
	if(mirrorTexture) ovr_DestroyMirrorTexture(HMD, reinterpret_cast<ovrTexture*>(mirrorTexture));

	// LE
	if(TextureSetLE)
	{
		ovr_DestroySwapTextureSet(HMD, TextureSetLE);
		TextureSetLE = nullptr;
	}

	if (tboLE)
	{
		glDeleteTextures(1, &tboLE);
		tboLE = 0;
	}

	if(fboLE)
	{
		glDeleteFramebuffers(1, &fboLE);
		fboLE = 0;
	}

	// RE
	if(TextureSetRE)
	{
		ovr_DestroySwapTextureSet(HMD, TextureSetRE);
		TextureSetRE = nullptr;
	}

	if (tboRE)
	{
		glDeleteTextures(1, &tboRE);
		tboRE = 0;
	}

	if(fboRE)
	{
		glDeleteFramebuffers(1, &fboRE);
		fboRE = 0;
	}

	ovr_Destroy(HMD);
	ovr_Shutdown();
	OVR::System::Destroy();
}

//////////////////////////////////////////////////////////////////////////////////////////
void InitializeDevices()
{
	/// Key callback
	glfwSetKeyCallback(window, key_callback);

	/// Mouse 
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetCursorPosCallback(window, Mouse_Callback);

	InitializeOculus();
}

void InitializeScene()
{
	//cube.Init();
	gScene.Init();

	gFBufferPtr = new Framebuffer();
	gFBufferPtr->FramebufferSetup();
}

//////////////////////////////////////////////////////////////////////////////////////////
int main(void)
{
	/// Initialize GLFW
	glfwInit();

	/// Set OpenGL context version & profile version
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, true);	

	/// Finally, Create a window
	window = glfwCreateWindow(gScreenWidth,gScreenHeight, "OpenGL Window", NULL, NULL);

	/// Make current context for this window
	glfwMakeContextCurrent(window);

	/// Init GLEW after window & context creation
	glewExperimental = true;
	glewInit();

	// Initialize Devices
	InitializeDevices();
	
	// Initialize Scene
	InitializeScene();

	//double lastFrameTime = glfwGetTime();

	/// Message Loop!
	while (!glfwWindowShouldClose(window))
	{
		/*double currFrameTime = glfwGetTime();
		float delta = (float)(currFrameTime - lastFrameTime);
		lastFrameTime = currFrameTime;*/

		GameLoop(tick);
		
		glfwSwapBuffers(window);
	}

	KillOculus();

	glfwTerminate();
	return 0;
}