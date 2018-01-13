/*
TODO:
	Allow input from the console that can restart, and stop. pause?
*/

// Only have this file once
#pragma once

#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif

// Include needed stuff
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "stb_image.h"

#include "shader.h"
#include "time.h"

#include <windows.h>
#include <iostream>

void framebufferSizeCallback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);

const unsigned int SCR_WIDTH = 1280;
const unsigned int SCR_HEIGHT = 720;

double seconds;
bool debug;
time_t start;
time_t end;
std::string consoleInput;

// Takes in launch options, useful for debugging
int main(int argc, char *argv[])
{
	Time timeFunctions;
	time(&start);  /* get current time; same as: timer = time(NULL)  */
	// Check if debug is enabled
	for (int i = 0; i < argc; i++) {
		// std::cout << timeFunctions.getCurrentTime() << "INFO: Launch arguments are: " << argv[i] << std::endl;
		// printf("INFO: argv[%d]: %s\n", i, argv[i]);
		if ((std::string) argv[i] == "-debug")
		{
			std::cout << timeFunctions.getCurrentTime() << "INFO: Debug enabled, all information will be printed to this console" << std::endl;
			std::cout << timeFunctions.getCurrentTime() << "INFO: ERROR indicates fatal error, and the application will terminate" << std::endl;
			std::cout << timeFunctions.getCurrentTime() << "INFO: INFO indicates useful information or non-fatal errors" << std::endl;
			std::cout << timeFunctions.getCurrentTime() << "INFO: SUCCESS indicates no fatal errors occurred for that process" << std::endl;
			debug = true;
			std::string str = "Kugelblitz Engine Console";
			SetConsoleTitle(str.c_str());
		}
		
	}
	if (debug == false) {
		FreeConsole();
	}

	// Initialize GLFW
	glfwInit();
	// Tell GLFW our OpenGL Version (3.3) 
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	// Tell GLFW our OpenGL Profile (Core)
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Create the window
	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Kugelblitz Engine", NULL, NULL);
	
	// Check if the window failed to create
	if (window == NULL)
	{
		glfwTerminate();
		return -1;
	} else if (window == NULL && debug == true) {
		// Output the GLFW error, and terminate the program
		std::cout << timeFunctions.getCurrentTime() << "ERROR: Failed to create GLFW window" << std::endl;
		glfwTerminate();
		std::cin.get();
		return -1;
	} else {
		std::cout << timeFunctions.getCurrentTime() << "SUCCESS: GLFW window created" << std::endl;
	}
	// Make the OpenGL context the current window
	glfwMakeContextCurrent(window);
	// Tell OpenGL our function to call when the window is resized
	glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);

	// GLAD: load all OpenGL function pointers
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << timeFunctions.getCurrentTime() << "ERROR: Failed to initialize GLAD" << std::endl;
		std::cin.get();
		return -1;
	} else {
		std::cout << timeFunctions.getCurrentTime() << "SUCCESS: Initialized GLAD" << std::endl;
	}

	// Build and compile our shader program
	Shader shader("shaders/vertex.glsl", "shaders/fragment.glsl", debug);

	// set up vertex data (and buffer(s)) and configure vertex attributes
	float vertices[] = {
		// positions          // colors           // texture coords
		0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f, // top right
		0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f, // bottom right
		-0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f, // bottom left
		-0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f  // top left 
	};

	unsigned int indices[] = {
		0, 1, 3, // first triangle
		1, 2, 3  // second triangle
	};

	// Create variables to store the IDs of the VAO, VBO 
	unsigned int VBO, VAO;

	// Create the IDs and put them into the VAO, VBO
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	// Nind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	/*
	In vertex.glsl, we say that our aPos attrib location is 0, and since we are dealing with
	position we input 0. We set the aPos to a vec3 which has 3 attributes. The data we are 
	inputting is a float. We do not want our data to be normalized. We have 6 pieces of data
	per vertex, so our stride is 6*so(float). Offset of where the pos data begins (none)
	*/
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	/*
	In vertex.glsl, we say that our aColor attrib location is 1, and since we are dealing 
	with color we input 0. We set the aColor to a vec3 which has 3 attributes. The data we 
	are inputting is a float. We do not want our data to be normalized. We have 6 pieces of 
	data per vertex, so our stride is 6*so(float). Offset of where the color data begins is 
	3 floats after the beginning of the array 
	*/
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	// You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
	// VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
	// glBindVertexArray(0);

	// Render loop
	while (!glfwWindowShouldClose(window))
	{
		// Input
		processInput(window);

		// Render
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		// Render the triangle
		shader.use();
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 3);

		// Swap frame buffers and poll IO events (keys pressed/released, mouse moved etc.)
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	// De-allocate all resources once they've outlived their purpose:
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);

	// Terminate GLFW, clearing all previously allocated GLFW resources
	glfwTerminate();
	time(&end);
	seconds = difftime(end, start);
	std::cout << timeFunctions.getCurrentTime() << "Time ran in seconds: " << seconds << std::endl;
	std::cin.get();
	return 0;
}

// Process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
void processInput(GLFWwindow *window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) 
	{
		glfwSetWindowShouldClose(window, true);
	}
}

// Whenever the window size changed (by OS or user resize) this callback function executes
void framebufferSizeCallback(GLFWwindow* window, int width, int height)
{
	//Make sure the viewport matches the new window dimensions; note that width and 
	// NOTE: Height will be significantly larger than specified on retina displays.
	glViewport(0, 0, width, height);
}