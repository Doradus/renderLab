struct PixelIn {
    float3 position : POSITION;
    float4 color : COLOR;
    float3 normal : NORMAL;
};

struct DirectionalLight {
    float3 color;
    bool enabled;
    float3 direction;
    float brightness;
};

struct PointLight {
    float3 color;
    bool enabled;
    float3 position;
    float range;
    float3 attenuation;
    float brightness;
};

struct Material {
    float3 albedo;
    float specularPower;
    float3 specularColor;
};

cbuffer FrameProperties : register(b0) {
    DirectionalLight directionalLights;
    PointLight pointLight;
    float3 eyePosition;
};

cbuffer MaterialProperties : register(b1) {
    Material material;
};

void ComputeDirectionalLight(float3 normal, float3 toEye, out float3 diffuseColor, out float3 specularColor) {
    diffuseColor = float3(0.0f, 0.0f, 0.0f);
    specularColor = float3(0.0f, 0.0f, 0.0f);

    float3 lightVector = -directionalLights.direction;
    float diffuseFactor = max(0.0f, dot(lightVector, normal));
    
    if (diffuseFactor > 0) {
        diffuseColor = material.albedo * directionalLights.brightness * directionalLights.color * diffuseFactor;
        float3 view = reflect(-lightVector, normal);
        float specFactor = pow(max(0.0f, dot(view, toEye)), material.specularPower);
        specularColor = specFactor * material.specularColor * directionalLights.brightness * directionalLights.color;
    }
}

void ComputePointLight(float3 normal, float3 position, float3 toEye, out float3 diffuseColor, out float3 specularColor) {
    diffuseColor = float3(0.0f, 0.0f, 0.0f);
    specularColor = float3(0.0f, 0.0f, 0.0f);

    float3 lightVector = pointLight.position - position;

    float distance = length(lightVector);

    if (distance > pointLight.range) {
        return;
    }

    lightVector /= distance;

    float diffuseFactor = max(0.0f, dot(lightVector, normal));

    if (diffuseFactor > 0) {
        diffuseColor = material.albedo * pointLight.brightness * pointLight.color * diffuseFactor;
        float3 view = reflect(-lightVector, normal);
        float specFactor = pow(max(0.0f, dot(view, toEye)), material.specularPower);
        specularColor = specFactor * material.specularColor * pointLight.brightness * pointLight.color;
    }

    float attenuation = 1.0f / dot(pointLight.attenuation, float3(1.0f, distance, distance * distance));
    //float attenuation = pointLight.attenuation.x + pointLight.attenuation.y * distance + pointLight.attenuation.z * distance * distance;
    diffuseColor *= attenuation;
    specularColor *= attenuation;
}

float4 BasicPixelShader(PixelIn vIn) : SV_TARGET {
    vIn.normal = normalize(vIn.normal);

    float3 diffuseColor = float3(0.0f, 0.0f, 0.0f);
    float3 specularColor = float3(0.0f, 0.0f, 0.0f);
    float3 toEye = normalize(eyePosition - vIn.position);

    float3 diffuse, specular;
    ComputeDirectionalLight(vIn.normal, toEye, diffuse, specular);
    diffuseColor += diffuse;
    specularColor += specular;

    ComputePointLight(vIn.normal, vIn.position, toEye, diffuse, specular);
    diffuseColor += diffuse;
    specularColor += specular;

    return float4(diffuseColor + specularColor, 1.0f);
}