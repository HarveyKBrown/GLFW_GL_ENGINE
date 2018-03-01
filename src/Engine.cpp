#include "Engine.h"
#include "EventManager.h"
#include "ShaderConstructor.h"
#include <iostream>
#include <glm/glm.hpp>
#include <glm/vec2.hpp>
#include "SceneA.h"
#include "SceneB.h"

/* Variables for temporary testing purposes */
int scene = 1;
SceneA sceneA;
SceneB sceneB;
//SceneC sceneC;
//SceneD sceneD;
//SceneE sceneE;

glm::vec2 hi;
int vertShad;
int fragShad;
int shaderProgram;
unsigned int VBO, VAO, EBO;


bool Engine::init(const char* title, int width, int height)
{
	glm::vec2 hi(1.0f, 2.0f);
	std::cout << hi.x << ", " << hi.y << std::endl;

	/* Initialise GLFW */
	if (!glfwInit())
	{
		std::cout << "Failed to initialise GLFW" << std::endl;
		return false;
	}
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	/* Create  Window */
	window = glfwCreateWindow(width, height, title, NULL, NULL);
	if (!window)
	{
		std::cout << "Failed to create window " << title << std::endl;
		return false;
	}
	glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);
	glfwMakeContextCurrent(window);

	/* Initialize InputHandler */
	glfwSetKeyCallback(window, EventManager::handleEvents);
	EventManager::registerEvent(GLFW_KEY_ESCAPE, [&] () { isRunning = false; });
	EventManager::registerEvent(GLFW_KEY_1, [&] () { scene = 0; });
	EventManager::registerEvent(GLFW_KEY_2, [&] () { scene = 1; });

	/* Initialise GLEW */
	glewInit();

	isRunning = true;
	return true;
}

bool Engine::initShaders()
{
	vertShad = ShaderConstructor::LoadShader("shaders/simple.vert", GL_VERTEX_SHADER);
	fragShad = ShaderConstructor::LoadShader("shaders/simple.frag", GL_FRAGMENT_SHADER);
	if (vertShad == 0 || fragShad == 0) return false;

	shaderProgram = ShaderConstructor::CreateShaderProgram(vertShad, fragShad);
	if (shaderProgram == 0) return 0;

	glDeleteShader(vertShad);
	glDeleteShader(fragShad);

	return true;
}

void Engine::calculateDeltaTime()
{
	LAST = NOW;
	NOW = glfwGetTime();
	deltaTime = NOW - LAST;
}

void Engine::update()
{
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	switch (scene)
	{
	case 0:
		sceneA.draw(VAO, VBO, EBO);
		break;
	case 1:
		sceneB.draw(VAO, VBO, EBO);
		break;
	}

	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Engine::render()
{
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	glUseProgram(shaderProgram);
	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	glfwSwapBuffers(window);
	glfwPollEvents();
}

void Engine::clean()
{
	glfwDestroyWindow(window);
	window = NULL;

	glfwTerminate();
}

void Engine::framebufferSizeCallback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}
