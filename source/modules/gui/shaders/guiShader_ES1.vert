R"(

#version 100

attribute vec2 position;
attribute vec4 color;

varying vec4 Color;

uniform mat4 projection;


void main()
{
	gl_Position = projection * vec4(position, -1.0, 1.0);
	Color = color;
}

)"