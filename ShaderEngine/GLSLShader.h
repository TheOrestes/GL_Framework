
#pragma once

#include "glew.h"
#include <string>

class GLSLShader
{
public:
	GLSLShader(const std::string& vertShader, const std::string& fragShader);
	~GLSLShader();

	void			Use();
	inline GLuint	GetShaderID(){ return shaderID; }

private:

	GLuint			shaderID;
	GLuint			LoadShader(const std::string& vertShader, const std::string& fragShader);
	bool			IsShaderCompiled(GLuint shaderID, const std::string& name);
};