#include "ShaderConstructor.h"

#include <iostream>
#include <fstream>
#include <string>
#include <GL/glew.h>

int ShaderConstructor::createShader(char* name, int shaderType)
{
	/* Load text file into string */
	std::ifstream shaderFile(name);
	std::string sShaderSource	(std::istreambuf_iterator<char>(shaderFile),
								(std::istreambuf_iterator<char>()		));
	const char* shaderSource = sShaderSource.c_str();

	int newShader = glCreateShader(shaderType);
	glShaderSource(newShader, 1, &shaderSource, NULL);
	glCompileShader(newShader);

	int success;
	char infoLog[512];
	glGetShaderiv(newShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(newShader, 512, NULL, infoLog);
		std::cout << "Failed to compile shader " << name << std::endl << infoLog << std::endl;
		return 0;
	}
	return newShader;
}

int ShaderConstructor::createShaderProgram(int vertexShader, int fragmentShader)
{

	return 0;
}
