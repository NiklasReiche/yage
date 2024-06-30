#version 330 core

in VS_OUT {
	vec3 FragPos;
	vec3 FragNormal;
	vec2 TexCoords;
} fs_in;

out vec4 FragColor;

uniform vec3 camPos;

struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
	sampler2D ambientMap;
	sampler2D diffuseMap;
	sampler2D specularMap;
	sampler2D shininessMap;
	sampler2D normalMap;
};
uniform Material material;

struct DirLight{
	vec3 direction;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};
uniform DirLight dirLights[5];
uniform int n_dirLights;

struct PointLight {
    vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
	float constant;
    float linear;
    float quadratic;
};
uniform PointLight pointLights[10];
uniform int n_pointLights;


struct ComputedMaterial {
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	float shininess;
};

vec3 calcDirLight(DirLight, vec3, vec3, ComputedMaterial);
vec3 calcPointLight(PointLight, vec3, vec3, vec3, ComputedMaterial);

void main(){
	vec3 N = normalize(fs_in.FragNormal) * texture(material.normalMap, fs_in.TexCoords).r; // just so normalMap doesn't get optimized away
	vec3 viewDir = normalize(camPos - fs_in.FragPos);

	ComputedMaterial mat;
	mat.ambient = texture(material.ambientMap, fs_in.TexCoords).rgb * material.ambient;
	mat.diffuse = texture(material.diffuseMap, fs_in.TexCoords).rgb * material.diffuse;
	mat.specular = texture(material.specularMap, fs_in.TexCoords).rgb * material.specular;
	mat.shininess = texture(material.shininessMap, fs_in.TexCoords).r * material.shininess;

	vec3 result = vec3(0.0);
	for (int i = 0; i < n_dirLights; i++) {
		result += calcDirLight(dirLights[i], N, viewDir, mat);
	}
	for (int i = 0; i < n_pointLights; i++) {
		result += calcPointLight(pointLights[i], N, viewDir, fs_in.FragPos, mat);
	}
	FragColor = vec4(result.rgb, 1.0f);
}

vec3 calcDirLight(DirLight light, vec3 normal, vec3 viewDir, ComputedMaterial material) {
	// --- Ambient Lighting
    vec3 ambient = light.ambient * material.ambient;
	// --- Diffuse Lighting
	vec3 lightDir = normalize(-light.direction);
	float diff = max(dot(normal, lightDir), 0.0);
	vec3 diffuse = light.diffuse * diff * material.diffuse;
	// --- Specular Lighting
    vec3 reflectDir = reflect(-lightDir, normal);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
	vec3 specular = light.specular * spec * material.specular;
	// --- Combined
	return ambient + diffuse + specular;
}

vec3 calcPointLight(PointLight light, vec3 normal, vec3 viewDir, vec3 FragPos, ComputedMaterial material) {
	// --- Falloff
	float distance    = length(light.position - FragPos);
	float attenuation = 1.0f / (light.constant + light.linear * distance + light.quadratic * (distance * distance));
	// --- Ambient Lighting
    vec3 ambient = light.ambient * material.ambient;
	// --- Diffuse Lighting
	vec3 lightDir = normalize(light.position - FragPos);
	float diff = max(dot(normal, lightDir), 0.0);
	vec3 diffuse = light.diffuse * diff * material.diffuse;
	// --- Specular Lighting
    vec3 reflectDir = reflect(-lightDir, normal);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
	vec3 specular = light.specular * spec * material.specular;
	// --- Combined
	ambient  *= attenuation; 
	diffuse  *= attenuation;
	specular *= attenuation;
	return ambient + diffuse + specular;
}
