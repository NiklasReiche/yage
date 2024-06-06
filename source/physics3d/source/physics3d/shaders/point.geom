#version 330 core

layout (points) in;
layout (points, max_vertices = 1) out;

out vec3 fColor;

uniform mat4 projection;
uniform mat4 view;

uniform vec3 point = vec3(0, 0, 0);
uniform float size = 5.0;
uniform vec3 color = vec3(1, 1, 1);

void main() {    
	gl_PointSize = size;
	fColor = color;

	gl_Position = projection * view * vec4(point, 1);
    EmitVertex();
    EndPrimitive();
}