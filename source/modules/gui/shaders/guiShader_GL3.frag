R"(

#version 330 core

in vec4 Color;
in vec2 TexCoords;

out vec4 color;

uniform sampler2D texture;

void main(){
	vec4 texColor = texture2D(texture, TexCoords);
	//color = (vec4(Color.rgb, 1) * Color.a) + (vec4(texColor.rgb, 1) * (1.0 - Color.a));
	color = Color * Color.a + texColor * (1.0 - Color.a);
	color = texColor;
}

)"