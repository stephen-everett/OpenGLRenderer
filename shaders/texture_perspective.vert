#version 330
// A vertex shader for perspective viewing of a mesh with normal vectors and texture coordinates.
layout (location=0) in vec3 vPosition;
layout (location=1) in vec3 vNormal; // use this for tbn matrix
layout (location=2) in vec2 vTexCoord;
// location 3 vtangent
layout (location=3) in vec3 vTangent;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

out vec2 TexCoord;
out vec3 Normal;
out vec3 FragWorldPos;

out mat3 TBN;

void main() {
    // Transform the position to clip space.
    gl_Position = projection * view * model * vec4(vPosition, 1.0);
    TexCoord = vTexCoord;

    // calculate tangents for TBN
    
    /*
    vec3 T = normalize(vec3(model* vec4(vTangent, 0.0)));
    vec3 N = normalize(vec3(model* vec4(vNormal, 0.0)));
    vec3 B = cross(N,T);
    mat3 TBN = mat3(T,B,N);
    

    // Transform the vertex normal to world space using the normal matrix.
    mat4 normalMatrix = transpose(inverse(model));
    Normal = mat3(normalMatrix) * vNormal;
    FragWorldPos = vec3(model* vec4(vPosition, 1.0));
}