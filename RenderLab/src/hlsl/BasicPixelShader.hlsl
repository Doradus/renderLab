struct PixelIn {
    float4 color : COLOR;
    float3 normal : NORMAL;
};

struct DirectionalLight {
    float3 color;
    bool enabled;
    float3 position;
    float brightness;
    float3 direction;
};

struct Material {
    float3 albedo;
};

cbuffer LightProperties : register(b0) {
    DirectionalLight dLights;
};

cbuffer MaterialProperties : register(b1) {
    Material material;
}

float4 BasicPixelShader(PixelIn vIn) : SV_TARGET {
    vIn.normal = normalize(vIn.normal);
    float3 lightVector = -dLights.direction;

    float diffuseFactor = max(0.0f, dot(lightVector, vIn.normal));

    return float4(material.albedo / 3.14f * dLights.brightness * dLights.color * diffuseFactor, 1.0f);
}