#define MAX_LIGHTS 8
#define DIRECTIONAL_LIGHT 0
#define POINT_LIGHT 1
#define SPOT_LIGHT 2

struct PixelIn {
    float3 position : POSITION;
    float4 color : COLOR;
    float3 normal : NORMAL;
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

float CalculateAttenuation(LightProperties light, float distance) {
   return 1.0f / dot(light.attenuation, float3(1.0f, distance, distance * distance));
}

float CalculateSpotIntensity(LightProperties light, float3 lightVector) {
    float outerBound = max(light.coneAngle, light.coneAngle + light.penumbraAngle);
    float innerBound = min(light.coneAngle, light.coneAngle + light.penumbraAngle);
    float maxIntensity = cos(innerBound);
    float minIntensity = cos(outerBound);

    return smoothstep(minIntensity, maxIntensity, dot(light.direction, -lightVector));
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

float4 BasicPixelShader(PixelIn vIn) : SV_TARGET {
    vIn.normal = normalize(vIn.normal);

    float3 diffuseColor = float3(0.0f, 0.0f, 0.0f);
    float3 specularColor = float3(0.0f, 0.0f, 0.0f);
    float3 toEye = normalize(eyePosition - vIn.position);

    
    float3 diffuse, specular;
    for (int i = 0; i < activeLights; i++) {
        switch (lights[i].type) {
            case DIRECTIONAL_LIGHT :
                ComputeDirectionalLight(vIn.normal, toEye, lights[i], diffuse, specular);
                diffuseColor += diffuse;
                specularColor += specular;
                break;
            case POINT_LIGHT:
                ComputePointLight(vIn.normal, vIn.position, toEye, lights[i],  diffuse, specular);
                diffuseColor += diffuse;
                specularColor += specular;
                break;
            case SPOT_LIGHT:
                ComputeSpotLight(vIn.normal, vIn.position, toEye, lights[i], diffuse, specular);
                diffuseColor += diffuse;
                specularColor += specular;
                break;
        }
    }

    diffuseColor *= material.albedo;
    specularColor *= material.specularColor;

    float3 finalDiffuse = saturate(diffuseColor);
    float3 finalSpecular = saturate(specularColor);

    return float4(diffuseColor + specularColor, 1.0f);
}