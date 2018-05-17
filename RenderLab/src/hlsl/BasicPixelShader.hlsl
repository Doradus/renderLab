struct PixelIn {
    float4 color : COLOR;
    float3 normal : NORMAL;
};

struct DirectionalLight {
    float3 color;
    bool enabled;
    float3 position;
    float3 direction;
};

cbuffer LightProperties : register(b0) {
    DirectionalLight dLights;
};

float4 BasicPixelShader(PixelIn vIn) : SV_TARGET {
    vIn.normal = normalize(vIn.normal);
    float3 lightVector = -dLights.direction;

    float diffueFactor = dot(lightVector, vIn.normal);

    return diffueFactor * float4(1.0f, 1.0f, 1.0f, 1.0f);
}