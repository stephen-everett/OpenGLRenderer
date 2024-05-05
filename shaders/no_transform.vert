#version 330
// A vertex shader for drawing meshes that are already defined in clip space.
layout (location=0) in vec3 vPosition;
// Uncomment the next lines, and do more, if your meshes also have normals and tex coords.
//layout (location=1) in vec3 vNormal;
//layout (location=2) in vec2 vTexCoord;
void main() {
    // Don't move the vertex at all; assume its local space == clip space.
    gl_Position = vec4(vPosition.x, vPosition.y, vPosition.z, 1) ;
}