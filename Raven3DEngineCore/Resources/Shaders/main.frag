#version 460
out vec4 FragColor;

in vec2 TexCoord;
in vec3 Normal;
in vec3 WorldPos;
in mat3 TBN;

#define LIGHT_TYPE_DIR 0
#define LIGHT_TYPE_POS 1
#define MAX_LIGHTS 50
#define PI 3.14159

struct Material {
    vec3 diffuseColor;
    vec3 specColor;
    vec3 ambientColor;
    vec3 emissiveColor;
    float opacity;
    float shininess;
    float shininessStrength;
    float metallicFactor;
    float roughnessFactor;
    bool useDiffTex;
    bool useSpecTex;
    bool useAmbTex;
    bool useEmisTex;
    bool useNormTex;
    sampler2D diffTex;
    sampler2D specTex;
    sampler2D ambTex;
    sampler2D emisTex;
    sampler2D normTex;
};


struct Light {
    vec3 color;
    vec3 posDir;
    int type;
    float intensity;
    float radius;
};


uniform Light[MAX_LIGHTS] uLights;
uniform int uNumLights;
uniform Material uMaterial;
uniform vec3 uCamPos;

float sqr (float val){
    return val * val;
}

float distributionGGX(vec3 N, vec3 H, float roughness) {
    float a2 = sqr(roughness);
    float NdotH = max(dot(N, H), 0.0);
    float NdotHSqr = sqr(NdotH);

    float num = a2;
    float denom = PI * sqr(NdotHSqr * (a2 - 1.0f) + 1.0f);

    return num / denom;
}

float geometrySchlickGGX(float NdotV, float roughness) {
    float num = NdotV;
    float denom = NdotV * (1.0f - roughness) + roughness;
    return num / denom;
}

float geometrySmith(vec3 N, vec3 V, vec3 L, float roughness){
    float NdotV = max(dot(N,V), 0.0);
    float NdotL = max(dot(N,L), 0.0);
    float ggx1 = geometrySchlickGGX(NdotV, roughness);
    float ggx2 = geometrySchlickGGX(NdotL, roughness);
    return ggx1 * ggx2;
}

vec3 fresnelSchlick(float cosTheta, vec3 FO) {
    return FO + (1.0f - FO) * pow(1.0f - cosTheta, 5.0f);
}

vec4 calcPBR(vec4 baseColor) {
    vec3 result = baseColor.rgb;
    float alpha = baseColor.a;


    vec3 N = normalize(Normal);
    if (uMaterial.useNormTex) {
        N = texture(uMaterial.normTex, TexCoord.xy).rgb;
        N = N * 2.0 - 1.0;
        N = normalize(TBN * N);
    }


    vec3 V = normalize(uCamPos - WorldPos);

    vec3 FO = vec3(0.04);
    FO = mix(FO, result, uMaterial.metallicFactor);

    vec3 LO = vec3(0.0f);
    for (int i = 0; i < uNumLights && i < MAX_LIGHTS; i++) {
        vec3 L = normalize(uLights[i].posDir - WorldPos);

        //radiance
        vec3 radiance;
        float attenuation = 1.0f;
        if (uLights[i].type == LIGHT_TYPE_DIR){
            attenuation = uLights[i].intensity;
            L = normalize(-uLights[i].posDir);
        } else if (uLights[i].type == LIGHT_TYPE_POS) {
            float distance = length(uLights[i].posDir - WorldPos);
            attenuation = uLights[i].intensity / (1 + sqr(distance/uLights[i].radius));
        }
        radiance = uLights[i].color * attenuation;

        vec3 H = normalize(V + L);

        //cook-torrence brdf
        float NDF = distributionGGX(N,H, uMaterial.roughnessFactor);
        float G = geometrySmith(N, V, L, uMaterial.roughnessFactor);
        vec3 F = fresnelSchlick(max(dot(H,V), 0.0f), FO);

        vec3 kS = F;
        vec3 kD = 1.0f - kS;
        kD *= 1.0 - uMaterial.metallicFactor;

        vec3 num = NDF * G * F;
        float denom = 4.0 * max(dot(N,V), 0.0f) * max(dot(N, L), 0.0f) + 0.0001f;
        vec3 specular = num/denom;

        //add to outgoing radiance
        float NdotL = max(dot(N, L), 0.0);
        LO += (kD*result / PI + specular) * radiance * NdotL;
    }

    vec3 ambient = uMaterial.ambientColor * result;
    vec3 color = ambient + LO;

    color = color / (color + vec3(1.0f));
    color = pow(color, vec3(1.0f/2.2f));

    result = color.rgb;

    return vec4(result, alpha);
}

void main() {
    vec4 color = vec4(uMaterial.diffuseColor, uMaterial.opacity);
    if (uMaterial.useDiffTex){
        color = color * texture(uMaterial.diffTex, TexCoord.xy);
    }
    FragColor = calcPBR(color);
}
