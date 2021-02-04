#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;

out vec3 FragPosition;
out vec3 FragNormal;

uniform mat4 model = mat4(1.0);
uniform mat4 view = mat4(1.0);
uniform mat4 projection = mat4(1.0);

void main() {
    gl_Position = projection * view * model * vec4(position, 1.0);
    //FragTexCoords = vec2(texCoord.x, 1.0f - texCoord.y);
    // -- Converting to World Space
    FragPosition = vec3(model * vec4(position, 1.0f));
    FragNormal = mat3(transpose(inverse(model))) * normal; // Calculation better be done outside of shader
}
