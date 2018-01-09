R"(

#version 330 core

struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
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


in vec3 FragPos;
in vec3 Normal;

out vec4 color;

uniform PointLight pointLights[10];
uniform int number_pointLight;
uniform Material material;
uniform vec3 viewPos;

vec3 calcPointLight(PointLight, vec3, vec3, vec3);

void main(){
	vec3 norm = normalize(Normal);
	vec3 viewDir = normalize(viewPos - FragPos);

	vec3 result = vec3(0.0);
	for (int i = 0; i < number_pointLight; i++) {
		result += calcPointLight(pointLights[i], norm, viewDir, FragPos);
	}
	color = vec4(result.rgb, 1.0f);
}

vec3 calcPointLight(PointLight light, vec3 normal, vec3 viewDir, vec3 FragPos){
	// --- Falloff
	float distance    = length(light.position - FragPos);
	float attenuation = 1.0f / (light.constant + light.linear * distance + light.quadratic * (distance * distance));
	// --- Ambient Lighting
    vec3 ambient = light.ambient * material.diffuse;
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

)"