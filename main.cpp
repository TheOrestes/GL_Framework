
#include <iostream>

#include "glew.h"
#include "GLFW\glfw3.h"

#include "Camera/Camera.h"
#include "Scene/Scene.h"
#include "Renderables/FrameBuffer.h"
#include "UI/UIManager.h"
#include "Helpers/LogManager.h"
#include "AntTweakBar.h"
#include "Globals.h"

//////////////////////////////////////////////////////////////////////////////////////////
GLFWwindow* window;
Scene		gScene;
Framebuffer* gFBufferPtr;  

const float tick			=	0.016667f;
bool  bFirstMouse			=	true;
bool  bMouseRotate			=	false;
float lastX					=	gScreenWidth / 2.0f;
float lastY					=	gScreenHeight / 2.0f;

//////////////////////////////////////////////////////////////////////////////////////////
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (!TwEventKeyGLFW3(window, key, scancode, action, mods))
	{
		if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
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
}

//////////////////////////////////////////////////////////////////////////////////////////
void MouseMovement_Callback(GLFWwindow* window, double xPos, double yPos)
{
	if (!TwEventCursorPosGLFW3(window, xPos, yPos))  // send event to AntTweakBar
	{ // event has not been handled by AntTweakBar
	  // your code here to handle the event
	  // ...
		if (bFirstMouse)
		{
			lastX = xPos;
			lastY = yPos;
			bFirstMouse = false;
		}

		GLfloat xoffset = xPos - lastX;
		GLfloat yoffset = lastY - yPos;
		lastX = xPos;
		lastY = yPos;

		// If mouse right click is pressed then we want to rotate the scene. 
		// Set the flag in MouseButton callback & check it here during 
		// MouseMovement callback!
		if(bMouseRotate)
		{
			Camera::getInstance().ProcessMouseMovement(xoffset, yoffset);
		}
	}
}

//////////////////////////////////////////////////////////////////////////////////////////
void MouseButton_Callback(GLFWwindow* window, int button, int action, int mods)
{
	if (!TwEventMouseButtonGLFW3(window, button, action, mods))
	{
		if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS)
			bMouseRotate = true;
		else
			bMouseRotate = false;
	}
}

//////////////////////////////////////////////////////////////////////////////////////////
void GameLoop(float tick)
{
	glfwPollEvents();

	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	gFBufferPtr->BeginRenderGeometryPass();
	gScene.Update(tick);
	gScene.Render();
	gFBufferPtr->EndRenderGeometryPass();

	gFBufferPtr->RenderDeferredLightingPass();

	gFBufferPtr->RenderPostProcessingPass();

	//gFBufferPtr->RenderBloomEffect();
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
	std::string text;

	LogManager::getInstance().WriteToConsole(LOG_RAW, "", "--------------------------- START ----------------------------");

	/// Initialize GLFW
	if (!glfwInit())
	{
		LogManager::getInstance().WriteToConsole(LOG_ERROR, "", "GLFW Initialization FAILED!");
		glfwTerminate();
		return 0;
	}

	/// Set OpenGL context version & profile version
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, true);	

	/// Finally, Create a window
	window = glfwCreateWindow(gScreenWidth,gScreenHeight, "OpenGL Window", NULL, NULL);
	if(!window)
	{
		text = "glfwCreateWindow FAILED...!!!";
		LogManager::getInstance().WriteToConsole(LOG_ERROR, "MainFunction", text);
	}
	else
	{
		text = "glfwCreateWindow Created...!!!";
		LogManager::getInstance().WriteToConsole(LOG_INFO, "MainFunction", text);
	}

	/// Make current context for this window
	glfwMakeContextCurrent(window);

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

	// - Directly redirect GLFW mouse button events to AntTweakBar 
	glfwSetMouseButtonCallback(window, (GLFWmousebuttonfun)MouseButton_Callback);
	// - Directly redirect GLFW mouse position events to AntTweakBar 
	glfwSetCursorPosCallback(window, (GLFWcursorposfun)MouseMovement_Callback);
	// - Directly redirect GLFW mouse wheel events to AntTweakBar 
	glfwSetScrollCallback(window, (GLFWscrollfun)TwEventScrollGLFW3);
	// - Directly redirect GLFW key events to AntTweakBar 
	glfwSetKeyCallback(window, (GLFWkeyfun)key_callback);
	// - Directly redirect GLFW char events to AntTweakBar 
	glfwSetCharCallback(window, (GLFWcharfun)TwEventCharModsGLFW3);

	/// Init GLEW after window & context creation
	glewExperimental = true;
	GLenum err = glewInit();
	if (GLEW_OK != err)
	{
		LogManager::getInstance().WriteToConsole(LOG_ERROR, "MainFunction", (char*)glewGetErrorString(err));
		return 0;
	}

	// Initialize UI Manager
	UIManager::getInstance().InitUIManager(window);

	// Initialize Scene
	InitializeScene();

	/// Message Loop!!!
	while (!glfwWindowShouldClose(window))
	{
		GameLoop(tick);
		UIManager::getInstance().Render();
		glfwSwapBuffers(window);
	}

	UIManager::getInstance().Kill();
	glfwTerminate();

	LogManager::getInstance().WriteToConsole(LOG_RAW, "", "--------------------------- END ----------------------------");

	return 0;
}