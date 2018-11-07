#define MAX_LIGHTS 8
#define DIRECTIONAL_LIGHT 0
#define POINT_LIGHT 1
#define SPOT_LIGHT 2

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

Texture2D diffuseTexture;
SamplerState textureSampler;

Texture2D normalTexture;
SamplerState normalSampler;

Texture2DArray shadowMap;
SamplerComparisonState shadowSampler;

TextureCube omniDirectionalShadowMap;
SamplerState trilinearSampler;


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

void ComputeDirectionalLight(float3 normal, float3 toEye, LightProperties light, out float3 diffuseColor, out float3 specularColor) {
    diffuseColor = float3(0.0f, 0.0f, 0.0f);
    specularColor = float3(0.0f, 0.0f, 0.0f);

    float3 lightVector = normalize(light.direction) * -1.0f;
    float diffuseFactor = max(0.0f, dot(lightVector, normal));
    
    if (diffuseFactor > 0) {
        diffuseColor = light.brightness * light.color * diffuseFactor;
        float3 view = reflect(-lightVector, normal);
        float specFactor = pow(max(0.0f, dot(view, toEye)), material.specularPower);
        specularColor = specFactor * light.brightness * light.color;
    }
}

void ComputePointLight(float3 normal, float3 position, float3 toEye, LightProperties light, out float3 diffuseColor, out float3 specularColor) {
    diffuseColor = float3(0.0f, 0.0f, 0.0f);
    specularColor = float3(0.0f, 0.0f, 0.0f);

    float3 lightVector = light.position - position;

    float distance = length(lightVector);

    if (distance > light.range) {
        return;
    }

    lightVector /= distance;

    float diffuseFactor = max(0.0f, dot(lightVector, normal));

    if (diffuseFactor > 0) {
        diffuseColor = light.brightness * light.color * diffuseFactor;
        float3 view = reflect(-lightVector, normal);
        float specFactor = pow(max(0.0f, dot(view, toEye)), material.specularPower);
        specularColor = specFactor * light.brightness * light.color;
    }

    float attenuation = CalculateAttenuation(light, distance);
    diffuseColor *= attenuation;
    specularColor *= attenuation;
}

void ComputeSpotLight(float3 normal, float3 position, float3 toEye, LightProperties light, out float3 diffuseColor, out float3 specularColor) {
    diffuseColor = float3(0.0f, 0.0f, 0.0f);
    specularColor = float3(0.0f, 0.0f, 0.0f);

    float3 lightVector = light.position - position;

    float distance = length(lightVector);

    if (distance > light.range) {
        return;
    }

    lightVector /= distance;

    float attenuation = CalculateAttenuation(light, distance);
    float spotIntensity = CalculateSpotIntensity(light, lightVector);

    float diffuseFactor = max(0.0f, dot(lightVector, normal));

    if (diffuseFactor > 0) {
        diffuseColor = light.brightness * light.color * diffuseFactor * attenuation * spotIntensity;
        float3 view = reflect(-lightVector, normal);
        float specFactor = pow(max(0.0f, dot(view, toEye)), material.specularPower);
        specularColor = specFactor * light.brightness * light.color * attenuation * spotIntensity;
    }
}

float3 SampledNormalToWorldSpace(float3 sampledNormal, float3 normal, float3 tangent) {
    float3 sampledNormalUncompressed = 2.0f * sampledNormal - 1.0f;

    float3 N = normal;
    float3 T = normalize(tangent - dot(tangent, N) * N);
    float3 B = cross(N, T);

    float3x3 TBN = float3x3(T, B, N);

    float3 transformedNormal = mul(sampledNormalUncompressed, TBN);

    return transformedNormal;
}

float4 Main(PixelIn vIn) : SV_TARGET {
    float3 normal = normalize(vIn.normal);

    float3 diffuseColor = float3(0.0f, 0.0f, 0.0f);
    float3 specularColor = float3(0.0f, 0.0f, 0.0f);
    float3 toEye = normalize(eyePosition - vIn.position);

    float3 sampledNormal = normalTexture.Sample(normalSampler, vIn.uv).rgb;
    float3 transformedSampledNormal = SampledNormalToWorldSpace(sampledNormal, normal, vIn.tangent);

    float3 diffuse, specular;
    for (int i = 0; i < activeLights; i++) {
        float shadowFactor = 1.0f;
        switch (lights[i].type) {
            case DIRECTIONAL_LIGHT :
                ComputeDirectionalLight(transformedSampledNormal, toEye, lights[i], diffuse, specular);
                if (lights[i].useShadow) {
                    shadowFactor = GetShadowFactor(vIn, lights[i]);
                }              
                diffuseColor += diffuse * shadowFactor;
                specularColor += specular * shadowFactor;
                break;
            case POINT_LIGHT:
                ComputePointLight(transformedSampledNormal, vIn.position, toEye, lights[i], diffuse, specular);
                if (lights[i].useShadow) {
                    shadowFactor = GetOmniDirectionalShadowFactor(vIn, lights[i]);
                }              
                diffuseColor += diffuse * shadowFactor;
                specularColor += specular * shadowFactor;
                break;
            case SPOT_LIGHT:
                ComputeSpotLight(transformedSampledNormal, vIn.position, toEye, lights[i], diffuse, specular);
                if (lights[i].useShadow)
                {
                    shadowFactor = GetShadowFactor(vIn, lights[i]);
                }
                diffuseColor += diffuse * shadowFactor;
                specularColor += specular * shadowFactor;
                break;
        }
    }

    float3 ambientLight = float3(0.2, 0.2, 0.25);
    float3 albedo = diffuseTexture.Sample(textureSampler, vIn.uv).rgb;

    //float3 debugg = normalTexture.Sample(normalSampler, vIn.uv).rgb;

    ambientLight *= albedo;
    diffuseColor *= albedo;
    specularColor *= material.specularColor;

    float3 finalDiffuse = saturate(diffuseColor);
    float3 finalSpecular = saturate(specularColor);

    return float4(ambientLight + finalDiffuse + finalSpecular, 1.0f);

}