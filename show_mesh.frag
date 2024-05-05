#version 330

layout (location=0) out vec4 FragColor;

in vec3 vColor;

uniform sampler2D baseTexture;

void main() {
	FragColor = vec4(vColor, 0.5);
}