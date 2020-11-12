#version 330 core

struct Material {
    sampler2D diffuse;
	sampler2D specular;
	sampler2D normalMap;
    float shininess;
}; 
struct DirLight{
	vec3 direction;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};
struct PointLight {
    vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
	float constant;
    float linear;
    float quadratic;
};
 
in vec3 Normal;
in vec2 TexCoord;
in vec3 FragPos;

out vec4 color;

uniform bool normalMapping;
uniform bool light = true;
uniform vec3 viewPos;
uniform Material material;
// ---
uniform DirLight dirLights[5];
uniform int number_dirLight;
uniform PointLight pointLights[10];
uniform int number_pointLight;

// --- Function Prototypes
vec3 calcDirLight(DirLight, vec3, vec3);
vec3 calcPointLight(PointLight, vec3, vec3, vec3);

void main(){
	vec3 norm = normalize(Normal);

	vec3 viewDir = normalize(viewPos - FragPos);

	vec3 result = vec3(0.0);
	for (int i = 0; i < number_dirLight; i++) {
		result += calcDirLight(dirLights[i], norm, viewDir);
	}
	for (int i = 0; i < number_pointLight; i++) {
		result += calcPointLight(pointLights[i], norm, viewDir, FragPos);
	}

	vec4 texcolor = vec4(result, texture(material.diffuse, TexCoord).a);
	if (texcolor.a < 0.1) {
		discard;
	}
	color = texcolor;
}
// ----- Light Type Functions
vec3 calcDirLight(DirLight light, vec3 normal, vec3 viewDir){
	// --- Ambient Lighting
    vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoord));
	// --- Diffuse Lighting
	vec3 lightDir = normalize(-light.direction);
	float diff = max(dot(normal, lightDir), 0.0);
	vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, TexCoord));
	// --- Specular Lighting
    vec3 reflectDir = reflect(-lightDir, normal);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
	vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoord));
	// --- Combined
	return ambient + diffuse + specular;
}
vec3 calcPointLight(PointLight light, vec3 normal, vec3 viewDir, vec3 FragPos){
	// --- Falloff
	float distance    = length(light.position - FragPos);
	float attenuation = 1.0f / (light.constant + light.linear * distance + light.quadratic * (distance * distance));
	// --- Ambient Lighting
    vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoord));
	// --- Diffuse Lighting
	vec3 lightDir = normalize(light.position - FragPos);
	float diff = max(dot(normal, lightDir), 0.0);
	vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, TexCoord));
	// --- Specular Lighting
    vec3 reflectDir = reflect(-lightDir, normal);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
	vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoord));
	// --- Combined
	ambient  *= attenuation; 
	diffuse  *= attenuation;
	specular *= attenuation;
	return ambient + diffuse + specular;
}
