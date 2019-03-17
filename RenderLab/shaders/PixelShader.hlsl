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

struct LightingResult {
    float3 diffuse;
    float3 specular;
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

cbuffer FrameProperties : register(b0) {
    LightProperties lights[MAX_LIGHTS];
    float3 eyePosition;
    int activeLights;
};

cbuffer MaterialProperties : register(b1) {
    Material material;
};

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
        shadowFactor = shadowMap.SampleCmpLevelZero(depthSampler, shadowTextureCoords, pixelDepth).r;
    }

    return shadowFactor;
}


float GetOmniDirectionalShadowFactor(PixelIn input, LightProperties light) {
    float3 distance = input.position - light.position;
    float nearest = SampleCubeMap(omniDirectionalShadowMap, trilinearSampler, distance).r;

    float3 absVec = abs(distance);
    float localZcomp = max(absVec.x, max(absVec.y, absVec.z));

    const float f = light.range;
    const float n = 1.0;
    float normZComp = (f + n) / (f - n) - (2.0 * f * n) / (f - n) / localZcomp;
    normZComp = (normZComp + 1.0) * 0.5;

    float shadowFactor = 1.0f;
    if (normZComp > nearest) {
        shadowFactor = 0.0f;
    } 

    return shadowFactor;
}

float4 GetAlbedo(PixelIn vIn) {
    %Albedo%
}

float4 GetNormal(PixelIn vIn) {
    %Normal%
}

float4 GetRoughness(PixelIn vIn) {
    %Roughness%
}

float3 GGXSpecular(float roughness, float3 specularColor, float LdotH, float NdotV, float NdotL, float NdotH)
{
    float a2 = roughness * roughness;

    float3 F = SchlickFresnel(specularColor, LdotH);
    float G = SmithForGGXMaskingAndShadowing(a2, NdotV, NdotL);
    float D = GGXDistribution(a2, NdotH);

    return (D * G) * F;
}

void ComputeLighting(float roughness, float3 N, float3 V, float3 L, float brightness, float fallOff, float3 lightColor, float shadowFactor, out LightingResult result) {
    float3 H = normalize(V + L);

    float NdotV = saturate(abs(dot(N, V)) + 1e-5f);
    float NdotH = saturate(dot(N, H));
    float NdotL = saturate(dot(N, L));
    float LdotH = saturate(dot(L, H));

    result.diffuse = DisneyDiffuseModel(roughness, NdotV, NdotL, LdotH) * brightness * fallOff * NdotL * shadowFactor;
    result.specular = GGXSpecular(roughness, material.specularColor, LdotH, NdotV, NdotL, NdotH) * brightness * fallOff * NdotL * lightColor * shadowFactor;
}

LightingResult ComputeDirectionalLight(PixelIn vIn, float3 N, float3 V, LightProperties light) {
    float3 L = normalize(light.direction) * -1.0f;
    LightingResult result;

    float shadowFactor = 1.0f;
    if (light.useShadow) {
        shadowFactor = GetShadowFactor(vIn, light);
    }

    ComputeLighting(GetRoughness(vIn).r, N, V, L, light.brightness, 1.0f, light.color, shadowFactor, result);

    return result;
}

LightingResult ComputePointLight(PixelIn vIn, float3 N, float3 position, float3 V, LightProperties light) {
    float3 L = light.position - position;
    float distance = length(L);
    LightingResult result;

    if (distance > light.range){
        result.diffuse = float3(0.0f, 0.0f, 0.0f);
        result.specular = float3(0.0f, 0.0f, 0.0f);
        return result;
    }

    float shadowFactor = 1.0f;
    if (light.useShadow) {
        shadowFactor = GetOmniDirectionalShadowFactor(vIn, light);
    }

    L /= distance;

    ComputeLighting(GetRoughness(vIn).r, N, V, L, light.brightness, CalculateAttenuation(light, distance), light.color, shadowFactor, result);

    return result;
}

LightingResult ComputeSpotLight(PixelIn vIn, float3 N, float3 position, float3 V, LightProperties light) {
    float3 L = light.position - position;
    float distance = length(L);
    LightingResult result;

    if (distance > light.range) {
        result.diffuse = float3(0.0f, 0.0f, 0.0f);
        result.specular = float3(0.0f, 0.0f, 0.0f);
        return result;
    }

    float shadowFactor = 1.0f;
    if (light.useShadow) {
        shadowFactor = GetShadowFactor(vIn, light);
    }

    L /= distance;
    float spotIntensity = CalculateSpotIntensity(light, L);

    ComputeLighting(GetRoughness(vIn).r, N, V, L, light.brightness, CalculateAttenuation(light, distance), light.color, shadowFactor, result);
    result.diffuse *= spotIntensity;
    result.specular *= spotIntensity;

    return result;
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
    float3 V = normalize(eyePosition - vIn.position);

    #if USE_NORMAL_MAP
    float4 sampledNormal = GetNormal(vIn);
    normal = TangentToWorldSpace(sampledNormal, normal, vIn.tangent);
    #endif

    float3 diffuse, specular;
    for (int i = 0; i < activeLights; i++) {
        LightingResult result;
        switch (lights[i].type) {
            case DIRECTIONAL_LIGHT :
                result = ComputeDirectionalLight(vIn, normal, V, lights[i]);
                break;
            case POINT_LIGHT:
                result = ComputePointLight(vIn, normal, vIn.position, V, lights[i]);
                break;
            case SPOT_LIGHT:
                result = ComputeSpotLight(vIn, normal, vIn.position, V, lights[i]);
                break;
            default:
                result.diffuse = float3(0.0f, 0.0f, 0.0f);
                result.specular = float3(0.0f, 0.0f, 0.0f);
                break;
        }

        diffuseColor += result.diffuse;
        specularColor += result.specular;
    }

    
    float3 ambientLight = float3(0.07, 0.07, 0.1);
    
    float3 reflectionVector = reflect(V, vIn.normal);
    //reflectionVector.g *= -1.0f;
 
    float3 sampleVector = reflectionVector;
    sampleVector.g *= -1.0f;
    float3 ambientLightColor = SampleCubeMap(environmentLight, trilinearSampler, sampleVector).rgb / PI;

    float r = ambientLightColor.r;
    float g = ambientLightColor.g;
    float b = ambientLightColor.b;

    ambientLightColor.r = b;
    ambientLightColor.b = r;
    

    /*
    float3 H = normalize(V + -reflectionVector);

    float NdotV = saturate(abs(dot(normal, V)) + 1e-5f);
    float NdotH = saturate(dot(normal, H));
    float NdotL = saturate(dot(normal, -reflectionVector));
    float LdotH = saturate(dot(-reflectionVector, H));
    */

    //float3 ambientLight = (GGXSpecular(GetRoughness(vIn).r, material.specularColor, LdotH, NdotV, NdotL, NdotH) * ambientLightColor * NdotL) / PI;

    float3 albedo = GetAlbedo(vIn).rgb;
    ambientLightColor *= albedo;
    ambientLight *= albedo;
    diffuseColor *= albedo;

    float3 finalDiffuse = saturate(diffuseColor);
    float3 finalSpecular = saturate(specularColor);

    return float4(ambientLightColor + diffuseColor + finalSpecular, 1.0f);
}