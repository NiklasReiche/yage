#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec2 texcoords;
layout (location = 2) in vec4 color;

out vec2 TexCoords;
out vec4 Color;

uniform mat4 projection = mat4(1.0);

void main()
{
    gl_Position = projection * vec4(position, 1.0);
    TexCoords = vec2(texcoords.x, texcoords.y);
    Color = color;
}
