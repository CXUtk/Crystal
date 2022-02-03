#version 330 core

in vec3 fNormal;
in vec2 fTexCoord;
out vec4 color;

void main() {
	color = vec4(0.5 + 0.5 * fNormal, 1.0);
}