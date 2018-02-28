#include "Engine.h"
#include "EventManager.h"
#include "ShaderConstructor.h"

#include <iostream>

/* Variables for temporary testing purposes */
int vertShad;
int fragShad;
int shaderProgram;
unsigned int VBO, VAO, EBO;



bool Engine::init(const char* title, int width, int height)
{
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
	/* Update Data or Switch Scenes */

    // set up vertex data (and buffer(s)) and configure vertex attributes
    // ------------------------------------------------------------------
    float vertices[] = {
         0.5f,  0.5f, 0.0f,  // top right
         0.5f, -0.5f, 0.0f,  // bottom right
        -0.5f, -0.5f, 0.0f,  // bottom left
        -0.5f,  0.5f, 0.0f   // top left
    };
    unsigned int indices[] = {  // note that we start from 0!
        0, 1, 3,  // first Triangle
        1, 2, 3   // second Triangle
    };
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
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
