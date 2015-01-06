
#pragma once

#include <iostream>
#include <GL/glew.h>
#include <string>
#include <fstream>

class GLSLParser
{
public:

	static GLSLParser& getInstance()
	{
		static GLSLParser instance;
		return instance;
	}

	~GLSLParser();

	GLuint		LoadShader(const std::string&, const std::string&);
	
private:
	std::string vertShader;
	std::string fragShader;

	GLSLParser();
	GLSLParser(const GLSLParser&);
	void operator=(const GLSLParser&);

	bool		IsShaderCompiled(GLuint shaderID, const std::string& name);
};