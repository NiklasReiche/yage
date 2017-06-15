#version 100
precision mediump float;

varying vec2 TexCoords;
varying vec4 Color;

uniform sampler2D text;

float width = 0.46;
float edge = 0.19;


void main()
{
	width = 0.5;
	edge = 0.1;
	float smoothing = 1.0/6.0;
	float dist = texture(text, TexCoords).r;
	//float alpha = smoothstep(width, width + edge, dist);
	float alpha = smoothstep(0.5 - smoothing, 0.5 + smoothing, dist); 

	gl_FragColor = vec4( Color.rgb, alpha * Color.a );
}