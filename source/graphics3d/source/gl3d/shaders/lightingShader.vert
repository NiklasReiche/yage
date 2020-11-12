R"(

#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 texCoord;
layout (location = 3) in vec3 tangent;

out vec3 FragPos;
out vec3 Normal;
out vec2 TexCoord;

uniform mat4 model = mat4(1.0);
uniform mat4 view = mat4(1.0);
uniform mat4 projection = mat4(1.0);

void main(){
	gl_Position = projection * view * model * vec4(position, 1.0);
	// -- Inverting texcoords
	TexCoord = vec2(texCoord.x, 1.0f - texCoord.y);
	// -- Converting to World Space
	FragPos = vec3(model * vec4(position, 1.0f));
	Normal = mat3(transpose(inverse(model))) * normal; // Calculation better be done outside of shader
}

)"