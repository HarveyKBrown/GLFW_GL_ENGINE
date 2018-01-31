#include "Engine.h"

void Engine::key_callback(GLFWwindow *window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		isRunning = false;
}

bool Engine::init(const char* title, int width, int height)
{
	if (!glfwInit()) { return false; }

	window = glfwCreateWindow(width, height, title, NULL, NULL);
	if (!window)
	{
		return false;
	}
	glfwMakeContextCurrent(window);

	//glfwSetKeyCallback(window, key_callback);

	isRunning = true;
	return true;
}

void Engine::update()
{
	float ratio;
	int width, height;
	glfwGetFramebufferSize(window, &width, &height);
	ratio = width / (float) height;
	glViewport(0, 0, width, height);
	glClear(GL_COLOR_BUFFER_BIT);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-ratio, ratio, -1.f, 1.f, 1.f, -1.f);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glRotatef((float) glfwGetTime() * 50.f, 0.f, 0.f, 1.f);
	glBegin(GL_TRIANGLES);
	glColor3f(1.f, 0.f, 0.f);
	glVertex3f(-0.6f, -0.4f, 0.f);
	glColor3f(0.f, 1.f, 0.f);
	glVertex3f(0.6f, -0.4f, 0.f);
	glColor3f(0.f, 0.f, 1.f);
	glVertex3f(0.f, 0.6f, 0.f);
	glEnd();
}

void Engine::render()
{
	glfwSwapBuffers(window);
	glfwPollEvents();
}

void Engine::clean()
{
	glfwDestroyWindow(window);
	window = NULL;

	glfwTerminate();
}
