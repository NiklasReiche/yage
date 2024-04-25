#version 330 core

in vec2 TexCoords;
in vec4 Color;

out vec4 color;

uniform sampler2D text;

const float edge = 0.5f;
const float spread = 4.0f;
const float scale = 16.0f / 100.0f;
const float smoothing = 0.25f / (spread * scale);

uniform float smoothing2 = 0;

void main()
{
	float dist = texture(text, TexCoords).r;
	float alpha = smoothstep(edge - smoothing2, edge + smoothing2, dist);
	color = vec4( Color.rgb, alpha * Color.a );
}
