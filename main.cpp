
#include <iostream>

#include "GL/glew.h"
#include "glfw3.h"

#include "Camera/Camera.h"
#include "Scene/Scene.h"
#include "Renderables/FrameBuffer.h"

#include "AntTweakBar.h"

//////////////////////////////////////////////////////////////////////////////////////////
GLFWwindow* window;
Scene		gScene;
Framebuffer* gFBufferPtr;  

TwBar* bar;
 
//GLCube		cube;

const float gScreenWidth	=	1280.0f;
const float gScreenHeight	=	800.0f;

const float tick			=	0.016667f;
bool  bFirstMouse			=	true;
float lastX					=	gScreenWidth / 2.0f;
float lastY					=	gScreenHeight / 2.0f;

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
	glfwPollEvents();

	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	gFBufferPtr->BeginRenderToFramebuffer();
	gScene.Update(tick);
	gScene.Render();
	gFBufferPtr->EndRenderToFramebuffer();

	gFBufferPtr->RenderFramebuffer();

	gFBufferPtr->RenderBloomEffect();
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

	/// Key callback
	glfwSetKeyCallback(window, key_callback);

	/// Mouse 
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	glfwSetCursorPosCallback(window, Mouse_Callback);

	/*
	// - Directly redirect GLFW mouse button events to AntTweakBar
	glfwSetMouseButtonCallback((GLFWmousebuttonfun)TwEventMouseButtonGLFW);
	// - Directly redirect GLFW mouse position events to AntTweakBar
	glfwSetCursorPosCallback((GLFWcursorposfun)TwEventMousePosGLFW);
	// - Directly redirect GLFW mouse wheel events to AntTweakBar
	glfwSetScrollCallback((GLFWscrollfun)TwEventMouseWheelGLFW);
	// - Directly redirect GLFW key events to AntTweakBar
	glfwSetKeyCallback((GLFWkeyfun)TwEventKeyGLFW);
	// - Directly redirect GLFW char events to AntTweakBar
	glfwSetCharCallback((GLFWcharfun)TwEventCharGLFW);*/

	/// Init GLEW after window & context creation
	glewExperimental = true;
	glewInit();

	TwInit(TW_OPENGL_CORE, nullptr);
	TwWindowSize(gScreenWidth, gScreenHeight);

	bar = TwNewBar("TestBar");
	TwDefine("This is the test bar rendered using Ant UI");

	// Add 'speed' to 'bar': it is a modifable (RW) variable of type TW_TYPE_DOUBLE. 
	// Its key shortcuts are [s] and [S].
	double speed = 0.3;
	bool wire = true;
	float bgColor[] = { 0.1f, 0.2f, 0.4f };
	unsigned char cubeColor[] = { 255, 0, 0, 128 }; 

	TwAddVarRW(bar, "speed", TW_TYPE_DOUBLE, &speed, " label='Rot speed' min=0 max=2 step=0.01 keyIncr=s keyDecr=S help='Rotation speed (turns/second)' ");
	TwAddVarRW(bar, "wire", TW_TYPE_BOOL32, &wire, " label='Wireframe mode' key=w help='Toggle wireframe display mode.' ");
	TwAddVarRW(bar, "bgColor", TW_TYPE_COLOR3F, &bgColor, " label='Background color' ");
	TwAddVarRW(bar, "cubeColor", TW_TYPE_COLOR32, &cubeColor, " label='Cube color' alpha help='Color and transparency of the cube.' ");

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
		TwDraw();
		
		glfwSwapBuffers(window);
	}

	TwTerminate();
	glfwTerminate();
	return 0;
}