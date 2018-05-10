#include "Engine.h"
#include "EventManager.h"
#include "ShaderConstructor.h"
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/vec3.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>
#include "GenericShapes.h"
#include "world.h"

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

enum camModes {
	DEMO,
	TOUR,
	SCREENSHOT
};

/* Camera Vars */
glm::mat4 freeCamPosition = glm::translate(freeCamPosition, glm::vec3(0.0f, 0.0f, -10.0f));
glm::vec3 cameraOrientation;
glm::vec3 cameraForces;
const float maxThrust = 10;
const float thrustGain = 1;
float cameraThrust;

float tourTimer;
float tourDuration = 20; //seconds?

/* Shader vars */
bool tourPaused = true;
camModes cameraMode = DEMO; // 0:Demo, 1:Tour, 2:Screenshot
glm::mat4 rotMatrix;
glm::mat4 cameraPosMatrix;
glm::mat4 screenshotCamPosition = glm::translate(screenshotCamPosition, glm::vec3(0.0f, 0.0f, -5.0f));
glm::mat4 tourCameraPosition;
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

	cameraPosMatrix = freeCamPosition;

	/* Initialize InputHandler */
	glfwSetKeyCallback(window, EventManager::handleEvents);
	EventManager::registerEvent(GLFW_KEY_ESCAPE, [&] () { isRunninsg = false; });
	EventManager::registerEvent(GLFW_KEY_Q, [&] () { isRunning = false; });
	/* Controls : Camera */
	EventManager::registerEvent(GLFW_KEY_P, [&]() { 
		if (cameraMode = DEMO)
		{
			cameraPosMatrix = screenshotPosition;
			cameraMode = SCREENSHOT;
		}
	}); //P to go to screenshot camera
	EventManager::registerEvent(GLFW_KEY_M, [&]() { 
		if (cameraMode = SCREENSHOT)
		{
			cameraPosMatrix = freeCamPosition;
			cameraMode = DEMO;
		}
	}); //Return to camera controls
	EventManager::registerEvent(GLFW_KEY_T, [&]() { 
		cameraPosMatrix = tourCameraPosition;
		tourPaused = false;
		cameraMode = TOUR;
	}); //Enter 'Tour' Mode Camera (20 second tours) 'catmul-Rom' spline?
	EventManager::registerEvent(GLFW_KEY_G, [&]() { 
		if (cameraMode = TOUR) 
		{
			cameraPosMatrix = freeCamPosition; 
			cameraMode = DEMO;
			tourPaused = true; 
		}
		
	}); //Exit tour (Maybe mask so it doesnt exit screenshot mode?)
	EventManager::registerEvent(GLFW_KEY_T, [&]() { 
		if (cameraMode = TOUR)
		{
			tourPaused = !tourPaused;
		}
	}); //Pauses tour : Tour must stop other controls

	EventManager::registerEvent(GLFW_KEY_PAGE_UP, [&]() { isRunning = false; }); //Rotate camera up
	EventManager::registerEvent(GLFW_KEY_PAGE_DOWN, [&]() { isRunning = false; }); //Rotate camera down
	EventManager::registerEvent(GLFW_KEY_UP, [&]() { cameraThrust = Mathf.Min(cameraThrust + thrustGain * deltaTime, maxThrust);  }); //Increase thrust (timedelta and limit)
	EventManager::registerEvent(GLFW_KEY_DOWN, [&]() { cameraThrust = Mathf.Max(cameraThrust - thrustGain * deltaTime, 0); }); //Decrease thrust (timeDelta and limit)
	EventManager::registerEvent(GLFW_KEY_LEFT, [&]() { isRunning = false; }); //Rotate camPos left
	EventManager::registerEvent(GLFW_KEY_RIGHT, [&]() { isRunning = false; }); //Rotate camPos right
	//Slowly reduce camera momentum over time? Store cam vec3 momentum, vec3 orientation, int thrust

	/* Initialise GLEW */
	glewInit();
	glEnable(GL_DEPTH_TEST);

	

	/* Create Scene Objects */
	shapes.push_back(new Sphere());
		shapes[0]->color = glm::vec3(1.f, 0.2f, 0.2f);

	shapes.push_back(new Cube());
		shapes[1]->color = glm::vec3(0.2f, 1.f, 0.2f);
		shapes[1]->orientation = glm::vec3(0.2f, 1.f, 0.2f);
		shapes[1]->scale = 1.3f;

	shapes.push_back(new Sphere());
		shapes[2]->scale = 200;
		shapes[2]->position = glm::vec3(0.f, -201.f, 0.f);
		shapes[2]->sphereIterations = 100;

	isRunning = true;
	return true;
}

bool Engine::initShaders()
{
	int sphereVert = ShaderConstructor::LoadShader("shaders/sphere.vert", GL_VERTEX_SHADER);
	int sphereFrag = ShaderConstructor::LoadShader("shaders/sphere.frag", GL_FRAGMENT_SHADER);
	int sphereTE = ShaderConstructor::LoadShader("shaders/sphere.te", GL_TESS_EVALUATION_SHADER);
	int sphereTC = ShaderConstructor::LoadShader("shaders/sphere.tc", GL_TESS_CONTROL_SHADER);

	int vertShad = ShaderConstructor::LoadShader("shaders/simple.vert", GL_VERTEX_SHADER);
	int fragShad = ShaderConstructor::LoadShader("shaders/simple.frag", GL_FRAGMENT_SHADER);

	shaderProgram = ShaderConstructor::CreateShaderProgram(sphereVert, sphereFrag, sphereTC, sphereTE);
	//shaderProgram = ShaderConstructor::CreateShaderProgram(vertShad, fragShad);

	glDeleteShader(sphereVert);
	glDeleteShader(sphereFrag);
	glDeleteShader(sphereTE);
	glDeleteShader(sphereTC);

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
	if (cameraMode = DEMO)
	{
		//Simulate Camera
		cameraForces = orientation * thrust; //TODO : Maybe get orientation by accessing camera quaternion, avoid gimbal lock?
		/* TEMP OVERWRITE */ cameraForces = glm::vec3(0, 0, 1) * thrust;
		freeCamPosition = glm::translate(freeCamPosition, cameraForces * DeltaTime);

		//TODO Check calculations, may want to maintain momentum after turning?
	}
	else if (camerMode = TOUR)
	{
		if (tourTimer < tourDuration && !tourPaused)
		{
			tourTimer += DeltaTime;
			//TODO : Simualte tour step
		}
	}
	/* 
	TODO : Simulate System
	*/
}

void Engine::render()
{
	glClearColor(0.5f, 0.2f, 0.1f, 1.0f);
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
	glViewport(0, 0, width, height);
}
