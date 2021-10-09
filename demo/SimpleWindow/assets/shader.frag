#version 430 core

in vec3 FragColor;

out vec4 color;

void main() {
    color = vec4(FragColor.xyz, 1.0f);
}
