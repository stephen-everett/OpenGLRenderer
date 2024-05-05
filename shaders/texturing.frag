#version 330
// A fragment shader for rendering a mesh that has a texture, but no lighting.
layout (location=0) out vec4 FragColor;

// Input from vertices: interpolated texture coordinate.
in vec3 FragWorldPos;
in vec3 Normal;
in vec2 TexCoord;

// Uniform from application: the texture sampler.
uniform sampler2D baseTexture;

uniform vec3 viewPos;
uniform vec3 ambientColor;
uniform vec3 directionalLight;
uniform vec3 directionalColor;
uniform vec4 material;


void main() {
    // temp uniforms
    //vec4 material = vec4(0.001, .001, 0.1, 32);
    //vec3 ambientColor = vec3(255,255,255);
    //vec3 directionalLight = vec3(0,-1,0); // direction of directional light
    //vec3 directionalColor = vec3(255,255,255);
    //vec3 viewPos = vec3(0,0,5);

    // ambient intensity
    vec3 ambientIntensity = vec3(0);
    ambientIntensity = material.x * ambientColor;

    // diffuse intensity
    vec3 diffuseIntensity = vec3(0);
    vec3 norm = normalize(Normal);
    vec3 lightDir = -directionalLight;

    // specular intensity
    vec3 specularIntensity = vec3(0);
    float lambertFactor = dot(norm, normalize(lightDir));
    if(lambertFactor > 0) {
        diffuseIntensity = material.y * directionalColor * lambertFactor;
        vec3 eyeDir = normalize(viewPos - FragWorldPos);
        vec3 reflectDir = normalize(reflect(-lightDir, norm));
        float spec = dot(reflectDir, eyeDir);
        if (spec > 0) {
            specularIntensity = material.z * directionalColor * pow(spec, material.w);
        }
    }
    

    //FragColor = texture(baseTexture, TexCoord);
    vec3 lightIntensity = ambientIntensity + diffuseIntensity + specularIntensity;
    FragColor = vec4(lightIntensity, 1) * texture(baseTexture, TexCoord);
    //FragColor = vec4(255,0,0,0);
}