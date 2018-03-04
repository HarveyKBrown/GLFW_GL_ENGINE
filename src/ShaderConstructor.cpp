#include "ShaderConstructor.h"

#include <iostream>
#include <fstream>
#include <string>
#include <GL/glew.h>

int ShaderConstructor::LoadShader(const char* FILENAME, int shaderType)
{
	/* Load text file into string */
	std::ifstream shaderFile(FILENAME);
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
		std::cout << "Failed to compile shader " << FILENAME << std::endl << infoLog << std::endl;
		return 0;
	}
	return newShader;
}

int ShaderConstructor::CreateShaderProgram(int vertexShader, int fragmentShader)
{
	int shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);

	int success;
	char infoLog[512];
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
		return 0;
	}
	return shaderProgram;
}

int ShaderConstructor::CreateShaderProgram(int vertexShader, int fragmentShader, int tessCont, int tessEval)
{
	int shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glAttachShader(shaderProgram, tessCont);
	glAttachShader(shaderProgram, tessEval);
	glLinkProgram(shaderProgram);

	int success;
	char infoLog[512];
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
		return 0;
	}
	return shaderProgram;
}
