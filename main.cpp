
#include <iostream>

#include "GL/glew.h"
#include "glfw3.h"

#include "ShaderEngine/GLSLParser.h"
#include "Renderables/GLCube.h"

//////////////////////////////////////////////////////////////////////////////////////////
GLFWwindow* window;
GLCube cube;
//GLSLQuad quad;

//////////////////////////////////////////////////////////////////////////////////////////
void Render()
{
	glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	cube.Update(0.6667f);
	cube.Render();
	//quad.Render();
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
	window = glfwCreateWindow(800,600, "OpenGL Window", NULL, NULL);

	/// Make current context for this window
	glfwMakeContextCurrent(window);

	/// Init GLEW after window & context creation
	glewExperimental = true;
	glewInit();
	
	cube.Init();
	//quad.Init();

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