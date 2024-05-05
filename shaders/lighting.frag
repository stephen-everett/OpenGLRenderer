#version 330
// A fragment shader for rendering fragments in the Phong reflection model.
layout (location=0) out vec4 FragColor;

// The mesh's base (diffuse) texture.
//uniform sampler2D baseTexture;

void main() {
    FragColor = vec4(255,0,0,0.1);
}