// Only have this file once
#pragma once

// Include GLAD
#include <glad/glad.h>

// Include standard file libraries
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

// Create the Shader class
class Shader
{
// Public parts of the class
public:
	// Shader ID
	unsigned int ID;
	// Constructor compiles shader on the fly, takes in vertex shader then fragment shader
	Shader(const char* vertexPath, const char* fragmentPath, const bool debugStatus)
	{
		m_debug = debugStatus;
		// Just create the variables if using a file stream
		std::string vertexCode = "#version 330 core\nlayout(location = 0) in vec3 aPos;\nlayout(location = 1) in vec3 aColor;\nout vec3 ourColor;\nvoid main()\n{\ngl_Position = vec4(aPos, 1.0);\nourColor = aColor;\n}";
		std::string fragmentCode = "#version 330 core\n out vec4 FragColor;\nin vec3 ourColor;\nvoid main()\n{\nFragColor = vec4(ourColor, 1.0);\n}";
		
		// Retriving the code from files, bad when shipping
		/*std::ifstream vShaderFile;
		std::ifstream fShaderFile;
		// Ensure ifstream objects can throw exceptions:
		vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
		fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
		try
		{
			// open files
			vShaderFile.open(vertexPath);
			fShaderFile.open(fragmentPath);
			std::stringstream vShaderStream, fShaderStream;
			// read file's buffer contents into streams
			vShaderStream << vShaderFile.rdbuf();
			fShaderStream << fShaderFile.rdbuf();
			// close file handlers
			vShaderFile.close();
			fShaderFile.close();
			// convert stream into string
			vertexCode = vShaderStream.str();
			fragmentCode = fShaderStream.str();
		}
		catch (std::ifstream::failure e)
		{
			std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
		}*/

		// Convert the code to C-strings
		const char* vShaderCode = vertexCode.c_str();
		const char * fShaderCode = fragmentCode.c_str();
		
		// Create 2 integers to put the ID in
		unsigned int vertex, fragment;

		// Vertex shader
		vertex = glCreateShader(GL_VERTEX_SHADER);
		// Shader object to compile, strings we are passing, code address, NULL
		glShaderSource(vertex, 1, &vShaderCode, NULL);
		// Compile the shader
		glCompileShader(vertex);
		checkCompileErrors(vertex, "VERTEX");
		
		// Fragment Shader
		fragment = glCreateShader(GL_FRAGMENT_SHADER);
		// Shader object to compile, strings we are passing, code address, NULL
		glShaderSource(fragment, 1, &fShaderCode, NULL);
		// Compile the shader
		glCompileShader(fragment);
		checkCompileErrors(fragment, "FRAGMENT");
		
		// Shader Program
		ID = glCreateProgram();
		// Attach the vertex and fragment shader to the main program
		glAttachShader(ID, vertex);
		glAttachShader(ID, fragment);
		// Link the program to the main thread
		glLinkProgram(ID);
		checkCompileErrors(ID, "PROGRAM");
		// Delete the shaders as they're linked into our program now and no longer necessary
		glDeleteShader(vertex);
		glDeleteShader(fragment);
	}
	// activate the shader
	void use()
	{
		glUseProgram(ID);
	}
	// Utility uniform functions
	void setBool(const std::string &name, bool value) const
	{
		glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
	}
	void setInt(const std::string &name, int value) const
	{
		glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
	}
	void setFloat(const std::string &name, float value) const
	{
		glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
	}

private:
	bool m_debug;
	// Utility function for checking shader compilation/linking errors
	void checkCompileErrors(unsigned int shader, std::string type)
	{
		int success;
		char infoLog[1024];
		if (type != "PROGRAM")
		{
			glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
			if (!success)
			{
				glGetShaderInfoLog(shader, 1024, NULL, infoLog);
				std::cout << "INFO: Shader compilation failed " << type << "\n" << infoLog << std::endl;
			} else if (m_debug == true) {
				std::cout << "INFO: Shader compilation succeeded" << std::endl;
			}
		}
		else
		{
			glGetProgramiv(shader, GL_LINK_STATUS, &success);
			if (!success)
			{
				glGetProgramInfoLog(shader, 1024, NULL, infoLog);
				std::cout << "INFO: Shader program linking error of type: " << type << "\n" << infoLog << std::endl;
			} else if(m_debug == true) {
				std::cout << "INFO: Shader program linking succeeded" << std::endl;
			}
		}
	}
};