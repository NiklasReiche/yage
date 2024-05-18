#version 330 core

in VS_OUT {
    vec3 FragPos;
    vec2 TexCoords;
    mat3 TBN;
} fs_in;

out vec4 FragColor;

uniform vec3 camPos;

struct Material {
    vec3 albedo;
    float metallic;
    float roughness;
    float ao;
    sampler2D albedoMap;
    sampler2D normalMap;
    sampler2D metallicRoughnessMap;
    sampler2D aoMap;
};
uniform Material material;

struct PointLight {
    vec3 position;
    vec3 color;
};
uniform PointLight pointLights[10];
uniform int n_pointLights;

const float PI = 3.14159265359;

vec3 fresnelSchlick(float cosTheta, vec3 F0);
float DistributionGGX(vec3 N, vec3 H, float roughness);
float GeometrySchlickGGX(float NdotV, float roughness);
float GeometrySmith(vec3 N, vec3 V, vec3 L, float roughness);

void main() {
    vec3 N = texture(material.normalMap, fs_in.TexCoords).rgb;
    N = N * 2.0 - 1.0; // map from [0,1] to [-1,1]
    N = normalize(fs_in.TBN * N); // TODO: use inverse of TBN to calculate lights/camera in T-space within vertex shader

    vec3 V = normalize(camPos - fs_in.FragPos);

    vec3 albedo = texture(material.albedoMap, fs_in.TexCoords).rgb * material.albedo;
    float metallic = texture(material.metallicRoughnessMap, fs_in.TexCoords).b * material.metallic;
    float roughness = texture(material.metallicRoughnessMap, fs_in.TexCoords).g * material.roughness;
    float ao = texture(material.aoMap, fs_in.TexCoords).r * material.ao;

    vec3 F0 = mix(vec3(0.04), albedo, metallic);

    vec3 Lo = vec3(0.0);
    for (int i = 0; i < n_pointLights; ++i) {
        vec3 L = normalize(pointLights[i].position - fs_in.FragPos);
        vec3 H = normalize(V + L);

        float distance = length(pointLights[i].position - fs_in.FragPos);
        float attenuation = 1.0 / (distance * distance);
        vec3 radiance = pointLights[i].color * attenuation;

        // cook-torrance brdf
        float NDF = DistributionGGX(N, H, roughness);
        float G = GeometrySmith(N, V, L, roughness);
        vec3 F = fresnelSchlick(max(dot(H, V), 0.0), F0);

        vec3 numerator = NDF * G * F;
        float denominator = 4.0 * max(dot(N, V), 0.0) * max(dot(N, L), 0.0) + 0.0001;
        vec3 specular = numerator / denominator;

        vec3 kS = F;
        vec3 kD = vec3(1.0) - kS;
        kD *= 1.0 - metallic;

        float NdotL = max(dot(N, L), 0.0);
        Lo += (kD * albedo / PI + specular) * radiance * NdotL;
    }

    vec3 ambient = vec3(0.03) * albedo * ao;
    vec3 color = ambient + Lo;

    // tone mapping (Reinhard operator)
    //color = color / (color + vec3(1.0));
    // gamma correction
    //color = pow(color, vec3(1.0/2.2));

    FragColor = vec4(color, 1.0);
}

vec3 fresnelSchlick(float cosTheta, vec3 F0)
{
    return F0 + (1.0 - F0) * pow(clamp(1.0 - cosTheta, 0.0, 1.0), 5.0);
}

float DistributionGGX(vec3 N, vec3 H, float roughness)
{
    float a      = roughness*roughness;
    float a2     = a*a;
    float NdotH  = max(dot(N, H), 0.0);
    float NdotH2 = NdotH*NdotH;

    float nom   = a2;
    float denom = (NdotH2 * (a2 - 1.0) + 1.0);
    denom = PI * denom * denom;

    return nom / denom;
}

float GeometrySchlickGGX(float NdotV, float roughness)
{
    float r = (roughness + 1.0);
    float k = (r*r) / 8.0;

    float nom   = NdotV;
    float denom = NdotV * (1.0 - k) + k;

    return nom / denom;
}

float GeometrySmith(vec3 N, vec3 V, vec3 L, float roughness)
{
    float NdotV = max(dot(N, V), 0.0);
    float NdotL = max(dot(N, L), 0.0);
    float ggx2  = GeometrySchlickGGX(NdotV, roughness);
    float ggx1  = GeometrySchlickGGX(NdotL, roughness);

    return ggx1 * ggx2;
}