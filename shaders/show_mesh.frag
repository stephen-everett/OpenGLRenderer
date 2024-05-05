#version 330

in vec3 vColor;

void main() {
	FragColor = vec4(vColor, 0.5);
}