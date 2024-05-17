#version 330 core

layout (location = 0) in vec2 position;
layout (location = 1) in vec4 color;
layout (location = 2) in vec2 texCoords;

out vec4 Color;
out vec2 TexCoords;

uniform mat4 projection = mat4(1.0);


void main(){
	gl_Position = projection * vec4(position, -1.0, 1.0);
	TexCoords = texCoords;
	Color = color;
}