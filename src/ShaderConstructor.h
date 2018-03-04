#ifndef SHADERCONSTRUCTOR_H
#define SHADERCONSTRUCTOR_H

class ShaderConstructor
{
	public:
		static int LoadShader(const char* FILENAME, int shaderType);
		static int CreateShaderProgram(int vertexShader, int fragmentShader);
		static int CreateShaderProgram(int vertexShader, int fragmentShader, int tessCont, int tessEval);
};

#endif
