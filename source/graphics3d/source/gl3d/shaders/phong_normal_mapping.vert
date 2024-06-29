#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec4 tangent;
layout (location = 3) in vec2 texCoord;

out VS_OUT {
    vec3 FragPos;
    vec2 TexCoords;
    mat3 TBN;
} vs_out;

layout (std140) uniform ProjectionView
{
    mat4 projection;
    mat4 view;
};

uniform mat4 model = mat4(1.0);

void main() {
    gl_Position = projection * view * model * vec4(position, 1.0);

    vs_out.FragPos = vec3(model * vec4(position, 1.0f));
    vs_out.TexCoords = vec2(texCoord.x, texCoord.y);

    vec3 T = normalize(vec3(model * vec4(tangent.xyz, 0.0)));
    vec3 N = normalize(vec3(model * vec4(normal, 0.0)));
    vec3 B = cross(N, T) * tangent.w;
    vs_out.TBN = mat3(T, B, N);
}
