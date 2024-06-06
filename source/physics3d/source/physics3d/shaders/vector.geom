#version 330 core

layout (points) in;
layout (line_strip, max_vertices = 2) out;

out vec3 fColor;

uniform mat4 projection;
uniform mat4 view;

uniform vec3 support = vec3(0, 0, 0);
uniform vec3 direction = vec3(0, 0, 0);
uniform vec3 color = vec3(1, 1, 1);

void main() {
	fColor = color;

	gl_Position = projection * view * vec4(support, 1);
    EmitVertex();
	gl_Position = projection * view * vec4(support + direction, 1);
	EmitVertex();

    EndPrimitive();
}