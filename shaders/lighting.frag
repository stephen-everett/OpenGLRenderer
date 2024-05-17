#version 330
// A fragment shader for rendering a mesh that has a texture, but no lighting.
layout (location=0) out vec4 FragColor;

// Input from vertices: interpolated texture coordinate.
in vec3 FragWorldPos;
in vec3 Normal;
in vec2 TexCoord;
in mat3 TBN;

// Uniform from application: the texture sampler.
uniform sampler2D baseTexture;
uniform sampler2D specularMap;
uniform sampler2D normalMap;

// directional light uniforms
uniform vec3 viewPos;
uniform vec3 ambientColor;
uniform vec3 directionalLight; // direction of light
uniform vec3 directionalColor;
uniform vec4 material;

// point light uniforms
uniform vec3 position1;
uniform vec3 position2;
uniform vec3 position3;
uniform vec3 position4;
uniform vec3 position5;

uniform float constant;
uniform float linear;
uniform float quadratic;
uniform vec4 point_material;

uniform vec3 p_color1;
uniform vec3 p_color2;
uniform vec3 p_color3;
uniform vec3 p_color4;
uniform vec3 p_color5;

uniform bool hasSpecMap;
uniform bool hasNormMap;
uniform bool useTBN;

// spot light uniforms
uniform vec4 spot_material;
uniform vec3 spotLightPosition;
uniform vec3 spotLightDirection;
uniform float spotLightCutOff;



vec3 CalcDirLight(vec4 mat, vec3 view, vec3 amColor, vec3 dlight, vec3 dcolor, vec3 normal, bool h_spec);
vec3 CalcPointLight(vec4 pmat,vec3 view, vec3 amColor, vec3 dlight, vec3 dcolor, vec3 position, vec3 color, vec3 normal, bool h_spec);
vec3 CalcSpotLight(vec4 mat, vec3 view, vec3 amColor, vec3 dlight, vec3 dcolor, vec3 normal, bool h_spec, float sCutOff);

void main() {
    vec3 normal = Normal;
    
    
    if (hasNormMap) {
        normal = texture(normalMap, TexCoord).rgb;
        normal = normal * 2.0 - 1.0;
        if(useTBN){
            normal = normalize(TBN * normal);
        }
        
    }
    vec3 directional_light = CalcDirLight(material,viewPos, ambientColor, directionalLight,directionalColor, normal, hasSpecMap); 
    vec3 point_light1 = CalcPointLight(point_material,  ambientColor, directionalLight,directionalColor,viewPos, position1, p_color1, normal, hasSpecMap);
    vec3 point_light2 = CalcPointLight(point_material,  ambientColor, directionalLight,directionalColor,viewPos, position2, p_color2, normal, hasSpecMap);
    vec3 point_light3 = CalcPointLight(point_material,  ambientColor, directionalLight,directionalColor,viewPos, position3, p_color3, normal, hasSpecMap);
    vec3 point_light4 = CalcPointLight(point_material,  ambientColor, directionalLight,directionalColor,viewPos, position4, p_color4, normal, hasSpecMap);
    vec3 point_light5 = CalcPointLight(point_material,  ambientColor, directionalLight,directionalColor,viewPos, position5, p_color5, normal, hasSpecMap);
    //vec3 spot_light = CalcSpotLight(material,viewPos, ambientColor, spotLightDirection,directionalColor, normal, hasSpecMap, spotLightCutOff); 

    vec3 total_light = directional_light + point_light1 + point_light2 + point_light3 + point_light4 + point_light5;// + spot_light;

    //vec3 total_light = spot_light;

    FragColor = vec4(total_light, 1) * texture(baseTexture, TexCoord);
}
   

vec3 CalcDirLight(vec4 mat, vec3 view, vec3 amColor, vec3 dlight, vec3 dcolor, vec3 normal, bool h_spec) {
    // ambient intensity
    vec3 ambientIntensity = vec3(0);
    ambientIntensity = mat.x * amColor;

    // diffuse intensity
    vec3 diffuseIntensity = vec3(0);
    vec3 norm = normalize(normal);
    vec3 lightDir = normalize(-dlight);
    //vec3 lightDir = normalize(dlight);

    // specular intensity
    vec3 specularIntensity = vec3(0);
    float lambertFactor = dot(norm, normalize(lightDir));
    if(lambertFactor > 0) {
        diffuseIntensity = mat.y * dcolor * lambertFactor;
        vec3 eyeDir = normalize(view - FragWorldPos);
        vec3 reflectDir = normalize(reflect(-lightDir, norm));
        float spec = dot(reflectDir, eyeDir);
        if (spec > 0) {
            if(h_spec){
                specularIntensity = texture(specularMap, TexCoord).x * dcolor * pow(spec, mat.w);
            }
            else {
                specularIntensity = mat.z * directionalColor * pow(spec, mat.w);
            }
            
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


vec3 CalcPointLight(vec4 pmat,vec3 view, vec3 amColor, vec3 dlight, vec3 dcolor, vec3 position, vec3 color, vec3 normal, bool h_spec) {

    float constant = 1.0;
    float linear = 0.35;
    float quadratic = 0.44;
    dlight = -dlight;

    float distance = length(position - FragWorldPos);
    float attenuation = 1.0 / (constant + linear * distance + quadratic * (distance * distance));

     // ambient intensity
    vec3 ambientIntensity = vec3(0);
    ambientIntensity = pmat.x * color;

    // diffuse intensity
    vec3 diffuseIntensity = vec3(0);
    vec3 norm = normalize(normal);
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
            if(h_spec){
                specularIntensity = texture(specularMap, TexCoord).x * dcolor * pow(spec, pmat.w);
            }
            else {
                specularIntensity = pmat.z * directionalColor * pow(spec, pmat.w);
            }
        }
    }

    specularIntensity *= attenuation;
    diffuseIntensity *= attenuation;
    ambientIntensity *= attenuation;

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


vec3 CalcSpotLight(vec4 mat, vec3 view, vec3 amColor, vec3 dlight, vec3 dcolor, vec3 normal, bool h_spec, float sCutoff) {
    float constant = 1.0;
    float linear = 0.35;
    float quadratic = 0.44;
    //dlight = -dlight;

    //float distance = length(spotLightPosition - FragWorldPos);
    //float attenuation = 1.0 / (constant + linear * distance + quadratic * (distance * distance));
    vec3 lightDir = normalize(dlight);
    float theta = dot(lightDir, normalize(-dlight));
    vec3 lightIntensity = vec3(0);

    if (theta > sCutoff) {
    
		
     // ambient intensity
    vec3 ambientIntensity = vec3(0);
    ambientIntensity = mat.x * amColor;

    // diffuse intensity
    vec3 diffuseIntensity = vec3(0);
    vec3 norm = normalize(normal);
    

    // specular intensity
    vec3 specularIntensity = vec3(0);
    float lambertFactor = dot(norm, normalize(lightDir));
    if(lambertFactor > 0) {
        diffuseIntensity = mat.y * dcolor * lambertFactor;
        vec3 eyeDir = normalize(view - FragWorldPos);
        vec3 reflectDir = normalize(reflect(-lightDir, norm));
        float spec = dot(reflectDir, eyeDir);
        if (spec > 0) {
           specularIntensity = mat.z * directionalColor * pow(spec, mat.w);
            
        }
    }

    // Spotlight intensity
    vec3 spotDir = normalize(spotLightPosition - FragWorldPos);
    float theta = dot(spotDir, normalize(-spotLightDirection));
    float epsilon = spotLightCutOff - 0.01;
    float intensity = clamp((theta - spotLightCutOff) / epsilon, 0.0, 1.0);

    //specularIntensity *= attenuation * intensity;
    //diffuseIntensity *= attenuation * intensity;
    //ambientIntensity *= attenuation * intensity;

    // Select what lights to enable
    lightIntensity += ambientIntensity + diffuseIntensity + specularIntensity; // all lights. normal rendering
    //vec3 lightIntensity = specularIntensity; // only specular
    //vec3 lightIntensity = ambientIntensity; // only ambient
    //vec3 lightIntensity = diffuseIntensity; // only diffuse
    //vec3 lightIntensity = ambientIntensity + specularIntensity; // ambient + spec
    //vec3 lightIntensity = ambientIntensity + diffuseIntensity; // ambient + diff
    //vec3 lightIntensity = specularIntensity + diffuseIntensity; // spec + diff
    }
    else{
    // ambient intensity
    vec3 ambientIntensity = vec3(0);
    ambientIntensity = mat.x * dcolor;
    lightIntensity = ambientIntensity;
    }

    return lightIntensity;
}
