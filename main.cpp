
#include <iostream>

#include "GL/glew.h"
#include "glfw3.h"

#include "Camera/Camera.h"
#include "ShaderEngine/GLSLParser.h"
#include "Renderables/GLCube.h"

//////////////////////////////////////////////////////////////////////////////////////////
GLFWwindow* window;
GLCube cube;//(glm::vec4(1,0,0,1)); 
GLCube cube2;//(glm::vec4(0,1,0,1));
GLCube cube3;//(glm::vec4(0,0,1,0));

const float gScreenWidth	=	800.0f;
const float gScreenHeight	=	600.0f;

const float tick			=	0.016667f;
bool  bFirstMouse			=	true;
float lastX					=	gScreenWidth / 2.0f;
float lastY					=	gScreenHeight / 2.0f;

//////////////////////////////////////////////////////////////////////////////////////////
void key_callback(GLFWwindow* window, int key, int scancode,	int action, int mods)
{
	if(key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
	{
		glfwWindowShouldClose(window);
	}

	if (key == GLFW_KEY_W && action == GLFW_REPEAT)
	{
		Camera::getInstance().ProcessKeyboard(CameraMovement::FORWARD, tick);	
	}

	if (key == GLFW_KEY_S && action == GLFW_REPEAT)
	{
		Camera::getInstance().ProcessKeyboard(CameraMovement::BACK, tick);	
	}

	if (key == GLFW_KEY_A && action == GLFW_REPEAT)
	{
		Camera::getInstance().ProcessKeyboard(CameraMovement::LEFT, tick);	
	}

	if (key == GLFW_KEY_D && action == GLFW_REPEAT)
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
void Render()
{
	glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	cube.Update(tick);
	cube.Render();

	cube2.Update(tick);
	cube2.Render();

	cube3.Update(tick);
	cube3.Render();
}

void InitializeScene()
{
	// Geometry
	cube.Init();

	cube2.Init();
	cube2.SetPosition(glm::vec3(-4,0,0));

	cube3.Init();
	cube3.SetPosition(glm::vec3(4,0,0));
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
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetCursorPosCallback(window, Mouse_Callback);

	/// Init GLEW after window & context creation
	glewExperimental = true;
	glewInit();
	
	// Initialize Scene
	InitializeScene();

	/// Message Loop!
	while (!glfwWindowShouldClose(window))
	{
		Render();

		/// Lookout for key press!
		glfwPollEvents();
		if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		{
			glfwSetWindowShouldClose(window, true);
		}
		
		glfwSwapBuffers(window);
	}

	glfwTerminate();
	return 0;
}