#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 texCoord;

out VS_OUT {
    vec3 FragPos;
    vec3 FragNormal;
    vec2 TexCoords;
} vs_out;

layout (std140) uniform ProjectionView
{
    mat4 projection;
    mat4 view;
};

uniform mat4 model = mat4(1.0);

void main() {
    gl_Position = projection * view * model * vec4(position, 1.0);
    vs_out.TexCoords = vec2(texCoord.x, texCoord.y);

    // -- Converting to World Space
    vs_out.FragPos = vec3(model * vec4(position, 1.0f));

    vs_out.FragNormal = mat3(transpose(inverse(model))) * normal; // Calculation better be done outside of shader
}