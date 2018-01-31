#ifndef ENGINE_H
#define ENGINE_H

#include <GLFW/glfw3.h>

class Engine {
	public:
		bool init(const char* title, int height, int width);
		void clean();
		void update();
		void render();

		static void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods);
	
		bool running() { return isRunning; };

	private:
		bool isRunning;

		GLFWwindow *window;
};

#endif
