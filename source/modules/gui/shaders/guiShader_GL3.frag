R"(

#version 330 core

in vec4 Color;
in vec2 TexCoords;

out vec4 color;

uniform sampler2D texture;

void main(){
	vec4 texColor = texture2D(texture, TexCoords);
	color = mix(texColor, Color, Color.a);
}

)"