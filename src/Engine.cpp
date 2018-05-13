#include "Engine.h"
#include "EventManager.h"
#include "ShaderConstructor.h"
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/vec3.hpp>
#include <glm/gtx/string_cast.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <vector>
#include <math.h>
#include "GenericShapes.h"

/* Shapes */
std::vector<Shape*> shapes;

/* Renderer Vars */
int shaderProgram;
int numberOfVertices;
unsigned int VBO, VAO, EBO;
glm::vec3 color;
int sphereIterations;
int numberOfVert;
float scale;

/* FreeCam Mode Vars */
enum camModes {
	DEMO,
	TOUR,
	SCREENSHOT
};
int cameraMode = DEMO;
const float camMaxForce = 3;
const float camAcceleration = 0.2f;
float camForce = 0;
const float camTurnSpeed = 4;
const float tourTimeMax = 20;
float tourTime;
float cameraDistance = -10;
glm::vec3 camOrientation = glm::vec3(0, 0, 0);

glm::vec3 camStartDemo = glm::vec3(0.0f, 0.0f, -10.0f);
glm::vec3 camStartTour = glm::vec3(0.0f, 0.0f, -15.0f);
glm::vec3 camStartScreenshot = glm::vec3(0.0f, 0.0f, -5.0f);


/* Shader vars */
glm::mat4 rotMatrix;
glm::mat4 cameraPosMatrix;
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
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
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
	/* FreeCam Controls */
	EventManager::registerEvent(GLFW_KEY_PAGE_UP, [&]() { 
		camOrientation = camOrientation + glm::vec3(1, 0, 0) * camTurnSpeed * (float)deltaTime; 
	}); //Rotate camera up
	EventManager::registerEvent(GLFW_KEY_PAGE_DOWN, [&]() { 
		camOrientation = camOrientation + glm::vec3(-1, 0, 0) * camTurnSpeed * (float)deltaTime; 
	}); //Rotate camera down
	EventManager::registerEvent(GLFW_KEY_UP, [&]() { 
		camForce = fmin(camForce + camAcceleration * deltaTime, camMaxForce);  
	}); //Increase thrust (timedelta and limit)
	EventManager::registerEvent(GLFW_KEY_DOWN, [&]() { 
		camForce = fmax(camForce - camAcceleration * deltaTime, -camMaxForce); 
	}); //Decrease thrust (timeDelta and limit)
	EventManager::registerEvent(GLFW_KEY_LEFT, [&]() { 
		camOrientation = camOrientation + glm::vec3(0, -1, 0) * camTurnSpeed * (float)deltaTime; 
	}); //Rotate camPos left
	EventManager::registerEvent(GLFW_KEY_RIGHT, [&]() { 
		camOrientation = camOrientation + glm::vec3(0, 1, 0) * camTurnSpeed * (float)deltaTime; 
	}); //Rotate camPos right
	/* Mode Switching */
	EventManager::registerEvent(GLFW_KEY_T, [&]() {
		cameraMode = TOUR; //TODO reset tourtime
		tourTime = 0;
	});
	EventManager::registerEvent(GLFW_KEY_G, [&]() {
		cameraMode = DEMO;
		cameraPosMatrix = glm::translate(glm::mat4(1.f), camStartDemo);
		camOrientation = glm::vec3();
		camForce = 0;
	});
	EventManager::registerEvent(GLFW_KEY_M, [&]() {
		cameraMode = DEMO;
		cameraPosMatrix = glm::translate(glm::mat4(1.f), camStartDemo);
		camOrientation = glm::vec3();
		camForce = 0;
	});
	EventManager::registerEvent(GLFW_KEY_P, [&]() {
		cameraMode = SCREENSHOT;
		cameraPosMatrix = glm::translate(glm::mat4(1.f), camStartScreenshot);
		cameraDistance = -5.0f;
		camOrientation = glm::vec3();
		camForce = 0;
	});

	/* Initialise GLEW */
	glewInit();
	glEnable(GL_DEPTH_TEST);

	/* Camera Positioning */
	cameraPosMatrix = glm::translate(glm::mat4(1.f), camStartDemo);

	/* Create Scene Objects */
	//Sun
	shapes.push_back(new Sphere());
		shapes[0]->color = glm::vec3(0.8f, 0.8f, 0.2f);
		shapes[0]->scale = 1.f;
	
	//Planet 1
	shapes.push_back(new Cube());
		shapes[1]->color = glm::vec3(0.2f, 0.6f, 0.2f);
		shapes[1]->orientation = glm::vec3(0.2f, 0.8f, 0.2f);
		shapes[1]->position = glm::vec3(2, 0, 0);
		shapes[1]->scale = 0.4f;

	//Planet 2
	shapes.push_back(new Cube());
		shapes[2]->color = glm::vec3(0.7f, 0.4f, 0.5f);
		shapes[2]->orientation = glm::vec3(0.2f, 0.0f, 0.8f);
		shapes[2]->position = glm::vec3(0, 0, -4);
		shapes[2]->scale = 0.4f;

	//Moon : Planet 1
	shapes.push_back(new Cube());
		shapes[3]->color = glm::vec3(0.8f, 0.4f, 0.6f);
		shapes[3]->orientation = glm::vec3(0.8f, 0.4f, 0.2f);
		shapes[3]->position = glm::vec3(2.5f, 0, 0);
		shapes[3]->scale = 0.1f;

	//Planet 2
	shapes.push_back(new Cube());
		shapes[4]->color = glm::vec3(0.4f, 0.4f, 0.8f);
		shapes[4]->orientation = glm::vec3(0.6f, 0.8f, 0.2f);
		shapes[4]->position = glm::vec3(0, 0, -4);
		shapes[4]->scale = 0.8f;
	
	shapes.push_back(new Sphere());
		shapes[5]->scale = 200;
		shapes[5]->position = glm::vec3(0.f, -210.f, 0.f);
		shapes[5]->color = glm::vec3(0.f, 0.5f, 0.1f);
		shapes[5]->sphereIterations = 100;

	isRunning = true;
	return true;
}

bool Engine::initShaders()
{
	int sphereVert = ShaderConstructor::LoadShader("shaders/sphere.vert", GL_VERTEX_SHADER);
	int sphereFrag = ShaderConstructor::LoadShader("shaders/sphere.frag", GL_FRAGMENT_SHADER);
	int sphereTE = ShaderConstructor::LoadShader("shaders/sphere.te", GL_TESS_EVALUATION_SHADER);
	int sphereTC = ShaderConstructor::LoadShader("shaders/sphere.tc", GL_TESS_CONTROL_SHADER);

	shaderProgram = ShaderConstructor::CreateShaderProgram(sphereVert, sphereFrag, sphereTC, sphereTE);

	glDeleteShader(sphereVert);
	glDeleteShader(sphereFrag);
	glDeleteShader(sphereTE);
	glDeleteShader(sphereTC);

	return true;
}

void Engine::calculateDeltaTime()
{
	LAST = NOW;
	NOW = glfwGetTime();
	deltaTime = NOW - LAST;
}

glm::vec3 Engine::orbitFunction(float radius, float orbitSpeed)
{
	float theta = (float)glfwGetTime() * orbitSpeed;
	float y = radius * sin(theta);
	float x = radius * cos(theta);
	return glm::vec3(x, 0, y);
}

void Engine::update()
{
	switch (cameraMode)
	{
	case SCREENSHOT: //Overload
	case DEMO:
		cameraDistance = fmax(fmin(-2.0f, cameraDistance + camForce), -80.0f);
		if (cameraDistance == -2.0f || cameraDistance == -80.0f) camForce = 0.f;
		cameraPosMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 1) * cameraDistance);
		if (glm::length(camOrientation) != 0)
		{
			cameraPosMatrix = glm::rotate(cameraPosMatrix, glm::length(camOrientation), camOrientation);
		}

		break;
	case TOUR:
		if (tourTime == 0)
			cameraPosMatrix = glm::translate(glm::mat4(1.f), camStartTour);
		if (tourTime > tourTimeMax)
		{
			cameraMode = DEMO;
			cameraPosMatrix = glm::translate(glm::mat4(1.f), camStartDemo);
			camOrientation = glm::vec3();
			camForce = 0;
		}
		tourTime += (float)deltaTime;

		/* Tour */
		cameraPosMatrix = glm::rotate(cameraPosMatrix, glm::radians(1.0f), glm::vec3(0, -1, 0));
		cameraPosMatrix = glm::rotate(cameraPosMatrix, glm::radians(0.2f), glm::vec3(0, 0, -1));
		cameraPosMatrix = glm::rotate(cameraPosMatrix, glm::radians(0.5f), glm::vec3(1, 0, 0));
		//cameraPosMatrix = glm::translate(cameraPosMatrix, glm::vec3(0, 0, 1));
		break;
	}
	shapes[0]->orientation += glm::vec3(0, 1, 0) * 0.1f * (float)deltaTime;
	shapes[1]->orientation += glm::vec3(0, 1, 1) * (float)deltaTime;
	shapes[1]->position = orbitFunction(2.f, -1.f);
	shapes[2]->orientation += glm::vec3(0, 1, 0.5f) * (float)deltaTime;
	shapes[2]->position = orbitFunction(6.f, 0.5f);
	shapes[3]->orientation += glm::vec3(0.2f, 0.1f, 0.6f) * 0.6f * (float)deltaTime;
	shapes[3]->position = orbitFunction(0.5f, 1.8f) + shapes[1]->position;
	shapes[4]->orientation += glm::vec3(0.7f, 0.2f, 0.2f) * (float)deltaTime;
	shapes[4]->position = orbitFunction(4.f, 0.2f);
}

void Engine::render()
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	bool fill = true;
	glPolygonMode(GL_FRONT_AND_BACK, fill ? GL_FILL : GL_LINE);

	/* START VAO LOOP */
	for (int i = 0; i < shapes.size(); i++) {
		/* Load Shape Data */
		Shape* currentShape = shapes[i];
		currentShape->draw(VAO, VBO, EBO);
		numberOfVertices = currentShape->getNumVectors();
		rotMatrix = currentShape->getPosMatrix();
		sphereIterations = currentShape->sphereIterations;
		color = currentShape->color;
		scale = currentShape->scale;

		/* Set Shader Uniforms */
		unsigned int modelLoc = glGetUniformLocation(shaderProgram, "uModel");
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, value_ptr(rotMatrix));
		unsigned int cameraPosition = glGetUniformLocation(shaderProgram, "uView");
		glUniformMatrix4fv(cameraPosition, 1, GL_FALSE, value_ptr(cameraPosMatrix));
		unsigned int perspectivePointer = glGetUniformLocation(shaderProgram, "uProjection");
		glUniformMatrix4fv(perspectivePointer, 1, GL_FALSE, value_ptr(projMatrix));
		unsigned int iterationsPointer = glGetUniformLocation(shaderProgram, "uIterations");
		glUniform1i(iterationsPointer, sphereIterations);
		unsigned int colorPointer = glGetUniformLocation(shaderProgram, "uColor");
		glUniform3f(colorPointer, color[0], color[1], color[2]);
		unsigned int scalePointer = glGetUniformLocation(shaderProgram, "uScale");
		glUniform1f(scalePointer, scale);

		glUseProgram(shaderProgram);
		glBindVertexArray(VAO);

		glPatchParameteri(GL_PATCH_VERTICES, 3);
		glDrawElements(GL_PATCHES, 1000, GL_UNSIGNED_INT, 0);

		glDeleteVertexArrays(sizeof(VAO), &VAO);
		glDeleteBuffers(sizeof(VBO), &VBO);
		glDeleteBuffers(sizeof(EBO), &EBO);
		/* END VAO LOOP */
	}

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
	projMatrix = glm::perspective(glm::radians(45.0f), (float)width / (float)height, 0.1f, 100.0f);
	glViewport(0, 0, width, height);
}
