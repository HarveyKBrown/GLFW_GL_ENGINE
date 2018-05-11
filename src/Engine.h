#ifndef ENGINE_H
#define ENGINE_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/vec3.hpp>

class Engine {
	public:
		bool init(const char* title, int height, int width);
		bool initShaders();
		void clean();
		void update();
		void render();
		void calculateDeltaTime();

		bool Running() { return isRunning && !glfwWindowShouldClose(window); };
		float DeltaTime() { return deltaTime; };

	private:
		static void framebufferSizeCallback(GLFWwindow* window, int width, int height);
		glm::vec3 orbitFunction(float radius, float orbitSpeed);
		bool isRunning = true;

		double deltaTime;
		double NOW;
		double LAST;

		GLFWwindow *window;
};

#endif
