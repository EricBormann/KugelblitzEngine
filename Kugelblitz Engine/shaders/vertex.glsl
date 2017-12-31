// VERTEX SHADER

// State the  OpenGL version (3.3)
#version 330 core

// in keyword indicates input of vertex attributes, we only leed a single one which is for the position, it gets inputed to aPos
// GLSL has vector datatype, we're in 3d so using vec3 (which has three values)
layout (location = 0) in vec3 aPos;

void main ()
{
	gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);
}