#version 330 core

in vec2 TexCoords;
in vec4 Color;

out vec4 color;

uniform sampler2D text;

uniform float spread = 1.0f;
uniform float scale = 1.0f;

const float edge = 0.5f;
float smoothing = 0.25f / (spread * scale);

void main()
{
	float dist = texture(text, TexCoords).r;
	float alpha = smoothstep(edge - smoothing, edge + smoothing, dist);
	color = vec4(Color.rgb, alpha * Color.a);
}
