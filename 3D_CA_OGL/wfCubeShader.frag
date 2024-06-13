#version 460 core
out vec4 FragColor;

in vec3 color;
in vec3 vbc;

const float lineWidth = 1.0;

float edgeFactor() {
	vec3 d = fwidth(vbc);
	vec3 f = step(d * lineWidth, vbc);
	return min(min(f.x, f.y), f.z);
}

void main() {
//	FragColor = vec4(color, 1.0);
	FragColor = vec4(min(vec3(edgeFactor()), color), 1.0);
}