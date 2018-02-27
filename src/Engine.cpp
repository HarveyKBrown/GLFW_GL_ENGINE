#include "Engine.h"
#include "EventManager.h"

#include <iostream>

bool Engine::init(const char* title, int width, int height)
{
	/* Initialise GLFW */
	if (!glfwInit())
	{ 
		std::cout << "Failed to initialise GLFW" << std::endl;
		return false; 
	}

	/* Create  Window */
	window = glfwCreateWindow(width, height, title, NULL, NULL);
	if (!window)
	{
		std::cout << "Failed to create window " << title << std::endl;
		return false;
	}
	glfwMakeContextCurrent(window);

	/* Initialize InputHandler */
	glfwSetKeyCallback(window, EventManager::handleEvents);
	EventManager::registerEvent(GLFW_KEY_ESCAPE, [&] () { isRunning = false; });

	/* Initialise GLEW */
	if (!glewInit())
   	{
		std::cout << "Failed to intitialise GLEW" << std::endl;
		return false;
	}

	glClearColor(1.0f, 0.0f, 0.0f, 1.0f);

	isRunning = true;
	return true;
}

bool Engine::initShaders()
{
	/* Create Shaders and Shader Programs for Seperate Scenes */
	return false;
}

void Engine::calculateDeltaTime()
{
	LAST = NOW;
	NOW = glfwGetTime();
	deltaTime = NOW - LAST;
}

void Engine::update()
{
	float ratio;
	int width, height;
	glfwGetFramebufferSize(window, &width, &height);
	ratio = width / (float) height;
	glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
	glViewport(0, 0, width, height);
	glClear(GL_COLOR_BUFFER_BIT);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-ratio, ratio, -1.f, 1.f, 1.f, -1.f);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glBegin(GL_TRIANGLES);
	glVertex3f(-0.6f, -0.4f, 0.f);
	glVertex3f(0.6f, -0.4f, 0.f);
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
