#version 330
// A vertex shader for rendering vertices with normal vectors and texture coordinates,
// which creates outputs needed for a Phong reflection fragment shader.
layout (location=0) in vec3 vPosition;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

out vec3 VertexColor;

void main() {
    // Transform the position to clip space.
    gl_Position = projection * view * model * vec4(vPosition, 1.0);
    VertexColor = vec3(255,0,0);
}