#version 460 core
layout (location = 0) in vec3 aPos;

out vec3 color;
out vec3 fragPos;  // To pass the fragment position to the fragment shader

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform vec3 cColor;

uniform vec3 lightPos;
uniform vec3 lightColor;
uniform bool useLighting;

void main() {
    gl_Position = projection * view * model * vec4(aPos, 1.0);
    fragPos = vec3(model * vec4(aPos, 1.0)); // Get the position of the fragment
    color = cColor;
}
