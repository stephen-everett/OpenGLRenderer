#version 330
// A fragment shader for rendering a mesh that has a texture, but no lighting.
layout (location=0) out vec4 FragColor;

// Input from vertices: interpolated texture coordinate.
in vec3 FragWorldPos;
in vec3 Normal;
in vec2 TexCoord;

// Uniform from application: the texture sampler.
uniform sampler2D baseTexture;
uniform sampler2D specularMap;

uniform vec3 viewPos;
uniform vec3 ambientColor;
uniform vec3 directionalLight;
uniform vec3 directionalColor;
uniform vec4 material;


void main() {
    /*
        Pointlight stuff
    */
    vec3 position = vec3(0);
    float constant = 1.0;
    float linear = 0.09;
    float quadratic = 0.032;

    float distance = length(position - FragWorldPos);
    float attenuation = 1.0 / (constant + linear * distance + quadratic * (distance * distance));


    // ambient intensity
    vec3 ambientIntensity = vec3(0);
    ambientIntensity = material.x * ambientColor;

    // diffuse intensity
    vec3 diffuseIntensity = vec3(0);
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(-directionalLight);

    // specular intensity
    vec3 specularIntensity = vec3(0);
    float lambertFactor = dot(norm, normalize(lightDir));
    if(lambertFactor > 0) {
        diffuseIntensity = material.y * directionalColor * lambertFactor;
        vec3 eyeDir = normalize(viewPos - FragWorldPos);
        vec3 reflectDir = normalize(reflect(-lightDir, norm));
        float spec = dot(reflectDir, eyeDir);
        if (spec > 0) {
            //specularIntensity = material.z * directionalColor * pow(spec, material.w);
            specularIntensity = texture(specularMap, TexCoord).x * directionalColor * pow(spec, material.w);
        }
    }
    
    ambientIntensity *= attenuation;
    diffuseIntensity *= attenuation;
    specularIntensity *= attenuation;


    // Select what lights to enable. 
    vec3 lightIntensity = ambientIntensity + diffuseIntensity + specularIntensity; // all lights. normal rendering
    //vec3 lightIntensity = specularIntensity; // only specular
    //vec3 lightIntensity = ambientIntensity; // only ambient
    //vec3 lightIntensity = diffuseIntensity; // only diffuse
    //vec3 lightIntensity = ambientIntensity + specularIntensity; // ambient + spec
    //vec3 lightIntensity = ambientIntensity + diffuseIntensity; // ambient + diff
    //vec3 lightIntensity specularIntensity + diffuseIntensity; // spec + diff
    FragColor = vec4(lightIntensity, 1) * texture(baseTexture, TexCoord);
   

}