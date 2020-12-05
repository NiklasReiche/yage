R"(
#version 330 core
layout (points) in;
layout (line_strip, max_vertices = 6) out;

out vec3 fColor;

uniform mat4 view = mat4(1.0);
uniform mat4 projection = mat4(1.0);
uniform float len = 1e20;

void main() {    
	gl_PointSize = 5.0;

	fColor = vec3(1.0, 0.0, 0.0);
	gl_Position = projection * view * gl_in[0].gl_Position;
    EmitVertex();
	gl_Position = projection * view * (gl_in[0].gl_Position + vec4(1.0*len, 0.0, 0.0, 1.0));
    EmitVertex();
    EndPrimitive();

	fColor = vec3(0.0, 1.0, 0.0);
	gl_Position = projection * view * gl_in[0].gl_Position;
    EmitVertex();
	gl_Position = projection * view * (gl_in[0].gl_Position + vec4(0.0, 1.0*len, 0.0, 1.0));
    EmitVertex();
    EndPrimitive();

	fColor = vec3(0.0, 0.0, 1.0);
	gl_Position = projection * view * gl_in[0].gl_Position;
    EmitVertex();
	gl_Position = projection * view * (gl_in[0].gl_Position + vec4(0.0, 0.0f, 1.0*len, 1.0));
    EmitVertex();
    EndPrimitive();
}
)"