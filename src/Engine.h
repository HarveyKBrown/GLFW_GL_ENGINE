#ifndef ENGINE_H
#define ENGINE_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>

class Engine {
	public:
		bool init(const char* title, int height, int width);
		bool initShaders();
		void clean();
		void update();
		void render();
		void calculateDeltaTime();

		bool Running() { return isRunning; };
		float DeltaTime() { return deltaTime; };

	private:
		GLuint LoadShader(const GLchar* source[], GLenum ShaderType);

		bool isRunning;

		double deltaTime;
		double NOW;
		double LAST;

		GLuint ProgramID = 0;

		GLFWwindow *window;
};

#endif
