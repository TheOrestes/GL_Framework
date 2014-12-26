
#include "GLSLParser.h"

// ---------------------------------------------------------------------------------------
/**
 * @brief  Constructor
 */
GLSLParser::GLSLParser()
{
	vertShader.empty();
	fragShader.empty();
}

// ---------------------------------------------------------------------------------------
/**
 * @brief Destructor
 */
GLSLParser::~GLSLParser()
{

}

// ---------------------------------------------------------------------------------------
/**
 * @brief Load shader & return shader program id
 *
 * This function reads shader code from the input string, compile the shader & create shader
 * program from it.
 * @param[in] vertex shader filename
 * @param[in] fragment shader filename
 * @param[]out GLuint shader program id
 * @return*/
GLuint GLSLParser::LoadShader(const std::string& vShader, const std::string& fShader)
{
	/// Read vertex shader code from the file...
	std::string vertexShaderSrc;
	std::ifstream vsStream(vShader.c_str(), std::ios::in);

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
	std::ifstream psStream(fShader.c_str(), std::ios::in);

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
	if(!IsShaderCompiled(vertexShaderID))
		return 0;

	/// Compile pixel shader...
	const char* ptrFragShaderSrc = pixelShaderSrc.c_str();
	glShaderSource(fragmentShaderID, 1, &ptrFragShaderSrc, NULL);
	glCompileShader(fragmentShaderID);

	// Check for shader compilation errors...
	if(!IsShaderCompiled(fragmentShaderID))
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

// ---------------------------------------------------------------------------------------
/**
 * @brief Check if shader compilation succeeded or failed?
 * @param[in] GLuint shaderID
 * @return GLboolean status flag
 */
bool GLSLParser::IsShaderCompiled(GLuint shaderID)
{
	GLint result = GL_FALSE;
	int infoLogLength;
	char infoLog[2048];

	glGetShaderiv(shaderID, GL_COMPILE_STATUS, &result);
	glGetShaderiv(shaderID, GL_INFO_LOG_LENGTH, &infoLogLength);

	if(!result)
	{
		glGetShaderInfoLog(shaderID, infoLogLength, NULL, infoLog);
		std::cout << "Error compiling shader :" << infoLog;
		return false;
	}

	return true;
}