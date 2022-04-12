#version 330 core

in vec4 fColor;
out vec4 color;

uniform float uLuminance;
uniform float uBase;

void main() {
	color = uBase + fColor * uLuminance;
}