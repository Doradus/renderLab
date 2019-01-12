#define MAX_LIGHTS 8
#define DIRECTIONAL_LIGHT 0
#define POINT_LIGHT 1
#define SPOT_LIGHT 2

#include <MaterialResources.hlsl>
#include <BRDF.hlsl>

struct PixelIn {
    float3 position : POSITION;
    float2 uv : UV;
    float3 normal : NORMAL;
    float3 tangent : TANGENT;
    float4 lightSpace [2] : LIGHT_SPACE_POSITION;
};

struct LightProperties {
    float3 color;
    bool enabled;
/* ------------------------- */
    float3 position;
    float range;
/* ------------------------- */
    float3 attenuation;
    float brightness;
/* ------------------------- */
    float3 direction;
    float type;
/* ------------------------- */
    float coneAngle;
    float penumbraAngle;
    bool useShadow;
    float shadowMapId;
};

struct Material {
    float3 albedo;
    float specularPower;
    float3 specularColor;
};

struct LightOut {
    float3 diffuse;
    float3 specular;
};

cbuffer FrameProperties : register(b0) {
    LightProperties lights[MAX_LIGHTS];
    float3 eyePosition;
    int activeLights;
};

cbuffer MaterialProperties : register(b1) {
    Material material;
};

SamplerComparisonState shadowSampler;
SamplerState trilinearSampler;
SamplerState textureSampler;

#if USE_NORMAL_MAP > 0
SamplerState normalSampler;
#endif

float CalculateAttenuation(LightProperties light, float distance) {
   return 1.0f / dot(light.attenuation, float3(1.0f, distance, distance * distance));
}

float CalculateSpotIntensity(LightProperties light, float3 lightVector) {
    float outerBound = max(light.coneAngle, light.coneAngle + light.penumbraAngle * 2.0f);
    float innerBound = min(light.coneAngle, light.coneAngle + light.penumbraAngle * 2.0f);
    float maxIntensity = cos(innerBound);
    float minIntensity = cos(outerBound);

    return smoothstep(minIntensity, maxIntensity, dot(light.direction, -lightVector));
}


float GetShadowFactor(PixelIn input, LightProperties light) {
    float3 shadowTextureCoords;
    float4 lightSpacePos = input.lightSpace[light.shadowMapId];
    shadowTextureCoords.x = 0.5f + (lightSpacePos.x / lightSpacePos.w * 0.5f);
    shadowTextureCoords.y = 0.5f - (lightSpacePos.y / lightSpacePos.w * 0.5f);
    shadowTextureCoords.z = light.shadowMapId;
    float pixelDepth = lightSpacePos.z / lightSpacePos.w;

    float shadowFactor = 1.0f;

    if ((saturate(shadowTextureCoords.x) == shadowTextureCoords.x) && (saturate(shadowTextureCoords.y) == shadowTextureCoords.y) && (pixelDepth > 0.0f)) {
        shadowFactor = shadowMap.SampleCmpLevelZero(shadowSampler, shadowTextureCoords, pixelDepth).r;
    }

    return shadowFactor;
}


float GetOmniDirectionalShadowFactor(PixelIn input, LightProperties light) {
    float3 distance = input.position - light.position;
    float nearest = omniDirectionalShadowMap.Sample(trilinearSampler, distance).r;

    float3 absVec = abs(distance);
    float localZcomp = max(absVec.x, max(absVec.y, absVec.z));

    const float f = light.range;
    const float n = 1.0;
    float normZComp = (f + n) / (f - n) - (2.0 * f * n) / (f - n) / localZcomp;
    normZComp = (normZComp + 1.0) * 0.5;

    float shadowFactor;
    if (normZComp > nearest) {
        shadowFactor = 0.0f;
    } else {
        shadowFactor = 1.0f;
    }

    return shadowFactor;
}

float3 GetAlbedo(PixelIn vIn) {
    %Albedo%
}

float4 GetNormal(PixelIn vIn) {
    %Normal%
}

float GetRoughness() {
    %Roughness%
}

float3 GGXSpecular(float3 V, float3 N, float3 L) {
    float a2 = GetRoughness() * GetRoughness();

    float3 H = normalize(V + L);

    float NdotV = abs(dot(N, V)) + 1e-5f;
    float NdotH = saturate(dot(N, H));
    float NdotL = saturate(dot(N, L));
    float LdotH = saturate(dot(L, H));

    float3 F = SchlickFresnel(material.specularColor, LdotH);
    float G = SmithForGGXMaskingAndShadowing(a2, NdotV, NdotL);
    float D = GGXDistribution(a2, NdotH);

    return (D * G) * F;
}

void ComputeDirectionalLight(float3 N, float3 V, LightProperties light, out float3 diffuseColor, out float3 specularColor)
{
    diffuseColor = float3(0.0f, 0.0f, 0.0f);
    specularColor = float3(0.0f, 0.0f, 0.0f);

    float3 L = normalize(light.direction) * -1.0f;
    float diffuseFactor = max(0.0f, dot(L, N));
    
    if (diffuseFactor > 0)
    {
        float3 H = normalize(V + L);
        float NdotV = abs(dot(N, V)) + 1e-5f;
        float NdotL = saturate(dot(N, L));
        float LdotH = saturate(dot(L, H));

        diffuseColor = DisneyDiffuseModel(GetRoughness(), NdotV, NdotL, LdotH) * light.brightness * light.color * diffuseFactor;
        float3 reflection = normalize(reflect(-L, N));
        float specFactor = max(0.0f, dot(reflection, V));
        specularColor = specFactor *GGXSpecular(V, N, L) * light.brightness * light.color;
    }
}

void ComputePointLight(float3 N, float3 position, float3 V, LightProperties light, out float3 diffuseColor, out float3 specularColor)
{
    diffuseColor = float3(0.0f, 0.0f, 0.0f);
    specularColor = float3(0.0f, 0.0f, 0.0f);

    float3 L = light.position - position;

    float distance = length(L);

    if (distance > light.range)
    {
        return;
    }

    L /= distance;

    float diffuseFactor = max(0.0f, dot(L, N));

    if (diffuseFactor > 0)
    {
        float3 H = normalize(V + L);
        float NdotV = abs(dot(N, V)) + 1e-5f;
        float NdotL = saturate(dot(N, L));
        float LdotH = saturate(dot(L, H));

        diffuseColor = DisneyDiffuseModel(GetRoughness(), NdotV, NdotL, LdotH) * light.brightness * light.color * diffuseFactor;
        float3 reflection = normalize(reflect(-L, N));
        float specFactor = max(0.0f, dot(reflection, V));
        //specularColor = specFactor * light.brightness * light.color;
        specularColor = specFactor * GGXSpecular(V, N, L) * light.brightness * light.color;
    }

    float attenuation = CalculateAttenuation(light, distance);
    diffuseColor *= attenuation;
    specularColor *= attenuation;
}

void ComputeSpotLight(float3 N, float3 position, float3 V, LightProperties light, out float3 diffuseColor, out float3 specularColor)
{
    diffuseColor = float3(0.0f, 0.0f, 0.0f);
    specularColor = float3(0.0f, 0.0f, 0.0f);

    float3 L = light.position - position;

    float distance = length(L);

    if (distance > light.range)
    {
        return;
    }

    L /= distance;

    float attenuation = CalculateAttenuation(light, distance);
    float spotIntensity = CalculateSpotIntensity(light, L);

    float diffuseFactor = max(0.0f, dot(L, N));

    if (diffuseFactor > 0)
    {
        float3 H = normalize(V + L);
        float NdotV = abs(dot(N, V)) + 1e-5f;
        float NdotL = saturate(dot(N, L));
        float LdotH = saturate(dot(L, H));

        diffuseColor = DisneyDiffuseModel(GetRoughness(), NdotV, NdotL, LdotH) * light.brightness * light.color * diffuseFactor * attenuation * spotIntensity;
        float3 reflection = normalize(reflect(-L, N));
        float specFactor = max(0.0f, dot(reflection, V));
        specularColor = specFactor * GGXSpecular(V, N, L) * light.brightness * light.color * attenuation * spotIntensity;
    }
}

float3 TangentToWorldSpace(float4 sampledNormal, float3 normal, float3 tangent) {
    float3 unpackedNormal = 2.0f * sampledNormal.rgb - 1.0f;

    /*
    float2 NormalXY = sampledNormal.rg;

    NormalXY = NormalXY * float2(2.0f, 2.0f) - float2(1.0f, 1.0f);
    float NormalZ = sqrt(saturate(1.0f - dot(NormalXY, NormalXY)));
    float3 unpackedNormal = float3(NormalXY.xy, NormalZ);
*/

    float3 N = normal;
    float3 T = normalize(tangent - dot(tangent, N) * N);
    float3 B = cross(N, T);

    float3x3 TBN = float3x3(T, B, N);

    float3 transformedNormal = mul(unpackedNormal, TBN);

    return transformedNormal;
}



float4 Main(PixelIn vIn) : SV_TARGET {
    float3 normal = normalize(vIn.normal);

    float3 diffuseColor = float3(0.0f, 0.0f, 0.0f);
    float3 specularColor = float3(0.0f, 0.0f, 0.0f);
    float3 toEye = normalize(eyePosition - vIn.position);

    #if USE_NORMAL_MAP
    float4 sampledNormal = GetNormal(vIn);
    normal = TangentToWorldSpace(sampledNormal, normal, vIn.tangent);
    #endif

    float3 diffuse, specular;
    for (int i = 0; i < activeLights; i++) {
        float shadowFactor = 1.0f;
        switch (lights[i].type) {
            case DIRECTIONAL_LIGHT :
                ComputeDirectionalLight(normal, toEye, lights[i], diffuse, specular);
                if (lights[i].useShadow) {
                    shadowFactor = GetShadowFactor(vIn, lights[i]);
                }              
                diffuseColor += diffuse * shadowFactor;
                specularColor += specular * shadowFactor;
                break;
            case POINT_LIGHT:
                ComputePointLight(normal, vIn.position, toEye, lights[i], diffuse, specular);
                if (lights[i].useShadow) {
                    shadowFactor = GetOmniDirectionalShadowFactor(vIn, lights[i]);
                }              
                diffuseColor += diffuse * shadowFactor;
                specularColor += specular * shadowFactor;
                break;
            case SPOT_LIGHT:
                ComputeSpotLight(normal, vIn.position, toEye, lights[i], diffuse, specular);
                if (lights[i].useShadow)
                {
                    shadowFactor = GetShadowFactor(vIn, lights[i]);
                }
                diffuseColor += diffuse * shadowFactor;
                specularColor += specular * shadowFactor;
                break;
        }
    }

    float3 ambientLight = float3(0.0, 0.0, 0.0);
    float3 albedo = GetAlbedo(vIn);
    ambientLight *= albedo;
    diffuseColor *= albedo;

    float3 finalDiffuse = saturate(diffuseColor);
    float3 finalSpecular = saturate(specularColor);

    return float4(ambientLight + diffuseColor + finalSpecular, 1.0f);
}