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

// directional light uniforms
uniform vec3 viewPos;
uniform vec3 ambientColor;
uniform vec3 directionalLight; // direction of light
uniform vec3 directionalColor;
uniform vec4 material;

// point light uniforms
uniform vec3 position1;
uniform vec3 position2;
uniform float constant;
uniform float linear;
uniform float quadratic;
uniform vec4 point_material;
uniform vec3 p_color1;
uniform vec3 p_color2;


vec3 CalcDirLight(vec4 mat, vec3 view, vec3 amColor, vec3 dlight, vec3 dcolor);
vec3 CalcPointLight(vec4 pmat,vec3 view, vec3 amColor, vec3 dlight, vec3 dcolor, vec3 position, vec3 color);


void main() {
 
    vec3 directional_light = CalcDirLight(material,viewPos, ambientColor, directionalLight,directionalColor); 
    vec3 point_light1 = CalcPointLight(point_material,  ambientColor, directionalLight,directionalColor,viewPos, position1, p_color1);
    vec3 point_light2 = CalcPointLight(point_material,  ambientColor, directionalLight,directionalColor,viewPos, position2, p_color2);

    vec3 total_light = directional_light + point_light1 + point_light2;
    
    FragColor = vec4(total_light, 1) * texture(baseTexture, TexCoord);
}
   

vec3 CalcDirLight(vec4 mat, vec3 view, vec3 amColor, vec3 dlight, vec3 dcolor) {
    // ambient intensity
    vec3 ambientIntensity = vec3(0);
    ambientIntensity = mat.x * amColor;

    // diffuse intensity
    vec3 diffuseIntensity = vec3(0);
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(-dlight);

    // specular intensity
    vec3 specularIntensity = vec3(0);
    float lambertFactor = dot(norm, normalize(lightDir));
    if(lambertFactor > 0) {
        diffuseIntensity = mat.y * dcolor * lambertFactor;
        vec3 eyeDir = normalize(view - FragWorldPos);
        vec3 reflectDir = normalize(reflect(-lightDir, norm));
        float spec = dot(reflectDir, eyeDir);
        if (spec > 0) {
            //specularIntensity = mat.z * directionalColor * pow(spec, mat.w);
            specularIntensity = texture(specularMap, TexCoord).x * dcolor * pow(spec, mat.w);
        }
    }

     // Select what lights to enable. 
    vec3 lightIntensity = ambientIntensity + diffuseIntensity + specularIntensity; // all lights. normal rendering
    //vec3 lightIntensity = specularIntensity; // only specular
    //vec3 lightIntensity = ambientIntensity; // only ambient
    //vec3 lightIntensity = diffuseIntensity; // only diffuse
    //vec3 lightIntensity = ambientIntensity + specularIntensity; // ambient + spec
    //vec3 lightIntensity = ambientIntensity + diffuseIntensity; // ambient + diff
    //vec3 lightIntensity specularIntensity + diffuseIntensity; // spec + diff

    return lightIntensity;
}


vec3 CalcPointLight(vec4 pmat,vec3 view, vec3 amColor, vec3 dlight, vec3 dcolor, vec3 position, vec3 color) {

    float constant = 1.0;
    float linear = 0.7;
    float quadratic = 1.8;

    float distance = length(position - FragWorldPos);
    float attenuation = 1.0 / (constant + linear * distance + quadratic * (distance * distance));

     // ambient intensity
    vec3 ambientIntensity = vec3(0);
    ambientIntensity = pmat.x * color;

    // diffuse intensity
    vec3 diffuseIntensity = vec3(0);
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(position -dlight);

    // specular intensity
    vec3 specularIntensity = vec3(0);
    float lambertFactor = dot(norm, normalize(lightDir));
    if(lambertFactor > 0) {
        diffuseIntensity = pmat.y * color * lambertFactor;
        vec3 eyeDir = normalize(view - FragWorldPos);
        vec3 reflectDir = normalize(reflect(-lightDir, norm));
        float spec = dot(reflectDir, eyeDir);
        if (spec > 0) {
            specularIntensity = pmat.z * directionalColor * pow(spec, pmat.w);
            //specularIntensity = texture(specularMap, TexCoord).x * color * pow(spec, pmat.w);
        }
    }

    specularIntensity *= attenuation;
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

    return lightIntensity;
}
