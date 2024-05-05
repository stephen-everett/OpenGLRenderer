#version 330
// A vertex shader for perspective viewing of a mesh with normal vectors and texture coordinates.
layout (location=0) in vec3 vPosition;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

out vec3 vColor;

void main() {
    gl_Position = projection * view * model * vec4(vPosition, 1.0);
    vec3 color = vec3(255,0,0);
    vColor = color;

}