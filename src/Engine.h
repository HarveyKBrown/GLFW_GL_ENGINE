#ifndef ENGINE_H
#define ENGINE_H

#include <GLFW/glfw3.h>

class Engine {
	public:
		bool init(const char* title, int height, int width);
		void clean();
		void update();
		void render();
		void calculateDeltaTime();

		bool running() { return isRunning; };
		float deltaTime() { return dTime; };

	private:
		bool isRunning;

		double dTime;
		double NOW;
		double LAST;

		GLFWwindow *window;
};

#endif
