#include "Engine.h"
#include "EventManager.h"
#include "ShaderConstructor.h"
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/vec3.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "SceneA.h"
#include "SceneB.h"


/* Variables for temporary testing purposes */
int scene = 0;
SceneA sceneA;
SceneB sceneB;
//SceneC sceneC;
//SceneD sceneD;
//SceneE sceneE;

int shaderProgram;
int cubeShaderProgram;
int sphereShaderProgram;
int numberOfVertices;
unsigned int VBO, VAO, EBO;

int numberOfVert;

glm::mat4 rotMatrix;
glm::mat4 posMatrix;
glm::mat4 projMatrix = glm::perspective(glm::radians(45.0f), (float)500 / (float)500, 0.1f, 100.0f);

bool Engine::init(const char* title, int width, int height)
{
	/* Initialise GLFW */
	if (!glfwInit())
	{
		std::cout << "Failed to initialise GLFW" << std::endl;
		return false;
	}
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // uncomment this statement to fix compilation on OS X
	#endif

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
	EventManager::registerEvent(GLFW_KEY_Q, [&] () { isRunning = false; });
	EventManager::registerEvent(GLFW_KEY_A, [&] () { scene = 0; });
	EventManager::registerEvent(GLFW_KEY_B, [&] () { scene = 1; });

	/* Initialise GLEW */
	glewInit();
	glEnable(GL_DEPTH_TEST);

	posMatrix = glm::translate(posMatrix, glm::vec3(0.0f, 0.0f, -5.0f));

	isRunning = true;
	return true;
}

bool Engine::initShaders()
{
	int vertShad = ShaderConstructor::LoadShader("shaders/simple.vert", GL_VERTEX_SHADER);
	int fragShad = ShaderConstructor::LoadShader("shaders/simple.frag", GL_FRAGMENT_SHADER);

	int sphereVert = ShaderConstructor::LoadShader("shaders/sphere.vert", GL_VERTEX_SHADER);
	int sphereFrag = ShaderConstructor::LoadShader("shaders/sphere.frag", GL_FRAGMENT_SHADER);
	int sphereTE = ShaderConstructor::LoadShader("shaders/sphere.te", GL_TESS_EVALUATION_SHADER);
	int sphereTC = ShaderConstructor::LoadShader("shaders/sphere.tc", GL_TESS_CONTROL_SHADER);


	cubeShaderProgram = ShaderConstructor::CreateShaderProgram(vertShad, fragShad);
	sphereShaderProgram = ShaderConstructor::CreateShaderProgram(sphereVert, sphereFrag, sphereTC, sphereTE);

	glDeleteShader(vertShad);
	glDeleteShader(fragShad);
	//glDeleteShader(sphereTessControl);
	//glDeleteShader(sphereTessEval);

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
	rotMatrix = glm::rotate(rotMatrix, (float) deltaTime * glm::radians(120.0f), glm::vec3(0.5f, 0.0f, 0.0f));

	switch (scene)
	{
	case 0:
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		sceneA.draw(VAO, VBO, EBO);
		shaderProgram = sphereShaderProgram;
		numberOfVertices = 36;
		break;
	case 1:
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		sceneA.draw(VAO, VBO, EBO);
		shaderProgram = sphereShaderProgram;
		numberOfVertices = 36;
		break;
	case 2:
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		sceneA.draw(VAO, VBO, EBO);
		shaderProgram = sphereShaderProgram;
		numberOfVertices = 36;
	case 3:
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		sceneA.draw(VAO, VBO, EBO);
		shaderProgram = sphereShaderProgram;
		numberOfVertices = 36;

	}

	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Engine::render()
{
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	unsigned int modelLoc = glGetUniformLocation(shaderProgram, "rM");
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, value_ptr(rotMatrix));

	unsigned int cameraPosition = glGetUniformLocation(shaderProgram, "pM");
	glUniformMatrix4fv(cameraPosition, 1, GL_FALSE, value_ptr(posMatrix));

	unsigned int perspectivePointer = glGetUniformLocation(shaderProgram, "perspective");
	glUniformMatrix4fv(perspectivePointer, 1, GL_FALSE, value_ptr(projMatrix));

	glUseProgram(shaderProgram);
	glBindVertexArray(VAO);
	switch (scene)
	{
	case 0:
		glDrawElements(GL_TRIANGLES, numberOfVertices, GL_UNSIGNED_INT, 0);
	case 1:
		glPatchParameteri(GL_PATCH_VERTICES, 3);
	}
	glDrawElements(GL_PATCHES, 1000, GL_UNSIGNED_INT, 0);
	glfwSwapBuffers(window);
	glfwPollEvents();

	glDeleteVertexArrays(sizeof(VAO), &VAO);
	glDeleteBuffers(sizeof(VBO), &VBO);
	glDeleteBuffers(sizeof(EBO), &EBO);
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
