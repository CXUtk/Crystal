#version 330 core
layout (location = 0) in vec4 vPos;
layout (location = 1) in vec2 vTexCoord;
layout (location = 2) in vec3 vNormal;

uniform mat4 M;
uniform mat4 MN;
uniform mat4 VP;

out vec2 fTexCoord;
out vec3 fNormal;

void main() {
    gl_Position = VP * M * vPos;
    fTexCoord = vTexCoord;
    fNormal = vec3(MN * vec4(vNormal, 0));
}

