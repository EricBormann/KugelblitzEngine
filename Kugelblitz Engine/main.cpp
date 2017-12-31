/*
	Kugelblitz Engine:
		To Do:
			- Switch to SDL
				~ Clean the main function up, many GLFW functions HAVE to be called from the main thread, SDL not so
		License:

			Copyright 2017 Eric Bormann

			Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, 
			including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, 
			subject to the following conditions:

			The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

			THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. 
			IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE 
			SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#pragma once
// Import the proper libraries, ensure that GLAD is imported prior to GLFW
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>

// define RELEASE/DEBUG depending on build type, this saves us log time
#define DEBUG

#ifdef DEBUG
// Only log errors if in DEBUG
#define LOG(x) std::cout << x << std::endl
#endif // DEBUG

#ifdef RELEASE
// Only log errors if in DEBUG
#define LOG(x)
#endif // RELEASE



// Indicate that these functions exist, for window resizing callback and processing keyboard input
void framebufferSizeCallback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);
void windowContext();
const char *vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"void main()\n"
"{\n"
"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
"}\0";

const char *fragmentShaderSource = "#version 330 core\n"
    "out vec4 FragColor;\n"
    "void main()\n"
    "{\n"
    "   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
	"}\n\0";

// Screen size variables
const unsigned int SCR_WIDTH = 1280;
const unsigned int SCR_HEIGHT = 720;

int main()
{
#pragma region Init
	// Removes the consoles
	// FreeConsole();

	// Console input
	// std::string consoleInput;

	// Initialize GLFW
	glfwInit();
	// Tell GLFW our OpenGL version "3.3"
	windowContext();

	// Create the GLFW window
	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Kugelblitz Engine", NULL, NULL);

	// Check if the window was not created, and output an error if there was
	if (window == NULL)
	{
		LOG("ERROR: Failed to create GLFW window");
		glfwTerminate();
		std::cin.get();
		return -1;
	}
	else {
		LOG("SUCCESS: Created GLFW window");
	}

	// Make the OpenGL context the current window
	glfwMakeContextCurrent(window);
	// Tell GLFW that if there is a resize, call the function framebufferSizeCallback	MUST BE CALLED IN MAIN
	glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);

	// GLAD loads the OpenGL function pointers (OS-specific) and GLFW tells us the glfwGetProcAddress to get the correct function for the OS we're compiling on (Windows 10)

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		LOG("ERROR: Failed to initialize GLAD");
		std::cin.get();
		return -1;
	}
	else {
		LOG("SUCCESS: Initialized GLAD");
	}
#pragma endregion

	// Vertex shader referenced by an ID
	unsigned int vertexShader;
	// Create the shaders
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	// Vertex to compile, amounnt of strings, actual code
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);
	//Check for vertex shader errors
	int success;
	char infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		LOG("ERROR: Shader vertex compilation failed:\n" << infoLog);
	} else {
		LOG("SUCCESS: Vertex Shader compilation completed");
	}
	
	// Fragment shader referenced by an ID
	unsigned int fragmentShader;
	// Set the fragment shader type to GL_FRAGMENT_SHADER
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	// Fragment shader to compile, amounnt of strings, actual code
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);

	// Triangle verticies
	float vertices[] = {
		-0.5f, -0.5f, 0.0f,
		0.5f, -0.5f, 0.0f,
		0.0f, 0.5f, 0.0f
	};
	// Vertex Buffer Object, stores verticies
	unsigned int VBO;
	// Generate the buffer
	glGenBuffers(1, &VBO);
	// Bind the VBO to the vertex buffer type "GL_ARRAY_BUFFER"
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	// Copy the data to the buffers memory for the currently bound GL_ARRAY_BUFFER
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// While GLFW is not being told to close, run this loop
	while (!glfwWindowShouldClose(window))
	{
		// Process user input for this window
		processInput(window);
		// Set the clear color to blue
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		// Clear the window
		glClear(GL_COLOR_BUFFER_BIT);

		// Swap frame buffers and poll window events
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	// At the end of the program, terminate GLFW and therefore the window
	glfwTerminate();
	LOG("The program ran successfully, have a good day.");
	std::cin.get();
	return 0;
}

/*
	FUNCTIONS
*/

// Provide information for the window
void windowContext() 
{
	// Tell GLFW our OpenGL version (3.3)
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	// Tell GLFW we want to use the core GLFW profile rather than the compatibility profile
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
}

// Process keyboard input for the specified window
void processInput(GLFWwindow *window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}

// Alter the OpenGL viewport as the window is resized
void framebufferSizeCallback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}