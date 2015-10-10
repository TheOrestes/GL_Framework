
#include <iostream>

#include "GL/glew.h"
#include "glfw3.h"

#include "Camera/Camera.h"
#include "Scene/Scene.h"
#include "Renderables/FrameBuffer.h"

#include "Oculus/OculusPlayer.h"

//////////////////////////////////////////////////////////////////////////////////////////
GLFWwindow* window;
Scene		gScene;
Framebuffer* gFBufferPtr;  
OculusPlayer* gOculus;

//GLCube		cube;

const float gScreenWidth	=	960.0f;
const float gScreenHeight	=	540.0f;

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
	/*glfwPollEvents();

	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);*/

	gOculus->UpdateOculusData();
	gOculus->BeginRenderOculusLE();
		// Render scene !!!
		gScene.Update(tick);
		gScene.Render();
	gOculus->EndRenderOculusLE();

	gOculus->BeginRenderOculusRE();
		// Render scene !!!
		gScene.Update(tick);
		gScene.Render();
	gOculus->EndRenderOculusRE();

	gOculus->FinalizeRenderOculus();
}

//////////////////////////////////////////////////////////////////////////////////////////
void InitializeDevices()
{
	/// Key callback
	glfwSetKeyCallback(window, key_callback);

	/// Mouse 
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetCursorPosCallback(window, Mouse_Callback);

	gOculus = new OculusPlayer();
	gOculus->InitializeOculus(gScreenWidth, gScreenHeight);
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

	delete gFBufferPtr;
	delete gOculus;

	glfwTerminate();
	return 0;
}