#version 460 core
layout (location = 0) in vec3 aPos;
//layout (location = 1) in vec3 aColor;

out vec3 color;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform vec3 cColor;

void main() {
	gl_Position = projection * view * model * vec4(aPos, 1.0);
	color = cColor;
}