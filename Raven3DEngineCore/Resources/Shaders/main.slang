#define LIGHT_TYPE_DIR 0
#define LIGHT_TYPE_POS 1
#define MAX_LIGHTS 50
#define PI 3.14159

struct VSInput 
{
    float3 aPos;
    float3 aNormal;
    float3 aTangent;
    float2 aUV_0;
}

struct Material {
    float3 diffuseColor;
    float3 specColor;
    float3 ambientColor;
    float3 emissiveColor;
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
    Sampler2D diffTex;
    Sampler2D specTex;
    Sampler2D ambTex;
    Sampler2D emisTex;
    Sampler2D normTex;
};

struct Light {
    float3 color;
    float3 posDir;
    int type;
    float intensity;
    float radius;
};

struct ShaderData 
{
    float4x4 uModel;
    float4x4 uView;
    float4x4 uProjection;
    Light[MAX_LIGHTS] uLights;
    int uNumLights;
    Material uMaterial;
    float3 uCamPos;
}


struct VSOutput 
{
    float2 TexCoord;
    float3 Normal;
    float3 WorldPos : SV_Position;
    float3x3 TBN;
}


[shader("vertex")]
VSOutput vertMain(VSInput input, uniform ShaderData *shaderData) 
{
    VSOutput output;
    output.Normal = mul((float3x3)mul(shaderData->uView, shaderData->uModel), input.aNormal);
    output.TexCoord = input.aUV_0;
    output.WorldPos = mul(shaderData->uProjection, mul(shaderData->uView, mul(shaderData->uModel, float4(input.aPos.xyz, 1.0)))).xyz;

    float3 T = normalize(float3((mul(shaderData->uModel, float4(input.aTangent, 0.0f))).xyz));
    float3 N = normalize(float3((mul(shaderData->uModel, float4(input.aNormal, 0.0f))).xyz));
    float3 B = normalize(cross(N, T));

    output.TBN = float3x3(T,B,N);

    return output;
}


float sqr (float val) {
    return val * val;
}

float distributionGGX(float3 N, float3 H, float roughness) 
{
    float a2 =  sqr(roughness);
    float NdotH = max(dot(N,H), 0.0f);
    float NdotHSqr = sqr(NdotH);

    float num = a2;
    float denom = PI * sqr(NdotHSqr * (a2 - 1.0f) + 1.0f);

    return num / denom;
}

float geometrySchlickGGX(float NdotV, float roughness) 
{
    float num = NdotV;
    float denom = NdotV * (1.0f - roughness) + roughness;
    return num / denom;
}

float geometrySmith(float3 N, float3 V, float3 L, float roughness) 
{
    float NdotV = max(dot(N,V), 0.0f);
    float NdotL = max(dot(N,L), 0.0f);
    float ggx1 = geometrySchlickGGX(NdotV, roughness);
    float ggx2 = geometrySchlickGGX(NdotL, roughness);
    return ggx1 * ggx2;
}

float3 fresnelSchlick(float cosTheta, float3 FO) {
    return FO + (1.0f - FO) * pow(1.0f - cosTheta, 5.0f);
}


float4 calcPBR(float4 color, Material *uMaterial, float3 Normal, float2 TexCoord, float3x3 TBN, float3 camPos, float3 worldPos, int uNumLights, Light[MAX_LIGHTS] uLights) 
{
    float3 result = color.rgb;
    float alpha = color.a;

    float3 N = normalize(Normal);
    if (uMaterial.useNormTex) {
        N = uMaterial.normTex.Sample(TexCoord.xy).rgb;
        N = N * 2.0 - 1.0;
        N = normalize(mul(TBN, N));
    }

    float3 V = normalize(camPos - worldPos);

    float3 FO = float3(0.04f);
    FO = lerp(FO, result, uMaterial.metallicFactor);

    float3 LO = float3(0.0f);
    for (int i = 0; i < uNumLights && i < MAX_LIGHTS; i++)
    {
        float3 L = normalize(uLights[i].posDir - worldPos);

        //radiance
        float3 radiance;
        float attenuation = 1.0f;
        if (uLights[i].type == LIGHT_TYPE_DIR){
            attenuation = uLights[i].intensity;
            L = normalize(-uLights[i].posDir);
        } else if (uLights[i].type == LIGHT_TYPE_POS) {
            float distance = length(uLights[i].posDir - worldPos);
            attenuation = uLights[i].intensity / (1 + sqr(distance/uLights[i].radius));
        }
        radiance = mul(uLights[i].color, attenuation);

        float3 H = normalize(V + L);

         //cook-torrence brdf
        float NDF = distributionGGX(N, H, uMaterial.roughnessFactor);
        float G = geometrySmith(N, V, L, uMaterial.roughnessFactor);
        float3 F = fresnelSchlick(max(dot(H,V), 0.0f), FO);

        float3 kS = F;
        float3 kD = 1.0f - kS;
        kD *= 1.0 - uMaterial.metallicFactor;

        float3 num = mul(NDF, mul(G, F));
        float denom = 4.0 * max(dot(N,V), 0.0f) * max(dot(N, L), 0.0f) + 0.0001f;
        float3 specular = num/denom;

        //add to outgoing radiance
        float NdotL = max(dot(N, L), 0.0);
        LO += (kD*result / PI + specular) * radiance * NdotL;

    }

    float3 ambient = uMaterial.ambientColor * result;
    float3 color = ambient + LO;

    color = color / (color + float3(1.0f));
    color = pow(color, float3(1.0f/2.2f));

    result = color.rgb;
    
    return float4(result, alpha);
}


[shader("fragment")]
float4 fragMain(VSOutput input, uniform ShaderData *uniformData) 
{
    float4 color = float4(uniformData->uMaterial.diffuseColor, uniformData->uMaterial.opacity);
    if (uniformData->uMaterial.useDiffTex) {
        color = color * uniformData->uMaterial.diffTex.Sample(input.TexCoord).rgba;
    }
    return calcPBR(color, &uniformData->uMaterial, input.Normal, input.TexCoord, input.TBN, uniformData->uCamPos, input.WorldPos, uniformData->uNumLights, uniformData->uLights);
}
