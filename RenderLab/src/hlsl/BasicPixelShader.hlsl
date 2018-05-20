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

struct Material {
    float3 albedo;
    float specularPower;
    float3 specularColor;
};

cbuffer FrameProperties : register(b0) {
    DirectionalLight dLights;
    float3 eyePosition;
};

cbuffer MaterialProperties : register(b1) {
    Material material;
};

float4 BasicPixelShader(PixelIn vIn) : SV_TARGET {
    vIn.normal = normalize(vIn.normal);

    float3 diffuseColor = float3(0.0f, 0.0f, 0.0f);
    float3 specularColor = float3(0.0f, 0.0f, 0.0f);

    float3 lightVector = -dLights.direction;

    float diffuseFactor = max(0.0f, dot(lightVector, vIn.normal));
    
    if (diffuseFactor > 0) {
        diffuseColor = material.albedo * dLights.brightness * dLights.color * diffuseFactor;

        float3 toEye = normalize(eyePosition - vIn.position);

        float3 view = reflect(-lightVector, vIn.normal);
        float specFactor = pow(max(0.0f, dot(view, toEye)), material.specularPower);
        specularColor = specFactor * material.specularColor * dLights.color;
    }

    return float4(diffuseColor + specularColor, 1.0f);
}