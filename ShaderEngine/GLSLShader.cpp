
#include "GLSLShader.h"
#include "../Helpers/LogManager.h"
#include <fstream>

//////////////////////////////////////////////////////////////////////////////////////////
GLSLShader::GLSLShader(const std::string& vertShader, const std::string& fragShader)
{
	shaderID = LoadShader(vertShader, fragShader);
}

//////////////////////////////////////////////////////////////////////////////////////////
GLSLShader::~GLSLShader()
{
	glDeleteProgram(shaderID);
}

//////////////////////////////////////////////////////////////////////////////////////////
GLuint	GLSLShader::LoadShader(const std::string& vertShader, const std::string& fragShader)
{
	/// Read vertex shader code from the file...
	std::string vertexShaderSrc;
	std::ifstream vsStream(vertShader.c_str(), std::ios::in);

	if(vsStream.is_open())
	{
		std::string line;
		while(std::getline(vsStream, line))
		{
			vertexShaderSrc += "\n" + line;
		}

		vsStream.close();
	}

	/// Read pixel shader code from the file...
	std::string pixelShaderSrc;
	std::ifstream psStream(fragShader.c_str(), std::ios::in);

	if(psStream.is_open())
	{
		std::string line;
		while(std::getline(psStream, line))
		{
			pixelShaderSrc += "\n" + line;
		}

		psStream.close();
	}

	/// Create shaders...
	GLuint vertexShaderID = glCreateShader(GL_VERTEX_SHADER);
	GLuint fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

	/// Compile Vertex shader...
	const char* ptrVertexShaderSrc = vertexShaderSrc.c_str();
	glShaderSource(vertexShaderID, 1, &ptrVertexShaderSrc, NULL);
	glCompileShader(vertexShaderID);

	// Check for shader compilation errors...
	if(!IsShaderCompiled(vertexShaderID, vertShader))
		return 0;

	/// Compile pixel shader...
	const char* ptrFragShaderSrc = pixelShaderSrc.c_str();
	glShaderSource(fragmentShaderID, 1, &ptrFragShaderSrc, NULL);
	glCompileShader(fragmentShaderID);

	// Check for shader compilation errors...
	if(!IsShaderCompiled(fragmentShaderID, fragShader))
		return 0;

	// Create shader program
	GLuint shaderProgramID = glCreateProgram();
	glAttachShader(shaderProgramID, vertexShaderID);
	glAttachShader(shaderProgramID, fragmentShaderID);
	glLinkProgram(shaderProgramID);

	glDeleteShader(vertexShaderID);
	glDeleteShader(fragmentShaderID);

	return shaderProgramID;
}

//////////////////////////////////////////////////////////////////////////////////////////
bool GLSLShader::IsShaderCompiled( GLuint shaderID, const std::string& name )
{
	GLint result = GL_FALSE;
	int infoLogLength;
	char infoLog[2048];

	glGetShaderiv(shaderID, GL_COMPILE_STATUS, &result);
	glGetShaderiv(shaderID, GL_INFO_LOG_LENGTH, &infoLogLength);

	if(!result)
	{
		glGetShaderInfoLog(shaderID, infoLogLength, NULL, infoLog);
		std::string err = "SHADER COMPILATION ERROR " + name + " : " + infoLog;
		LogManager::getInstance().WriteToConsole(LOG_ERROR, err);
		return false;
	}

	return true;
}

//////////////////////////////////////////////////////////////////////////////////////////
void	GLSLShader::Use()
{
	glUseProgram(shaderID);
}


