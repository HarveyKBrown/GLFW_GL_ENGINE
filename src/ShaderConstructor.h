#ifndef SHADERCONSTRUCTOR_H
#define SHADERCONSTRUCTOR_H

class ShaderConstructor
{
	public:
		int createShader(char* name, int shaderType);
		int createShaderProgram(int vertexShader, int fragmentShader);
};

#endif
