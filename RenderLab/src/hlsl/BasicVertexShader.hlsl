#include <CommonVS.hlsl>

struct VertexIn {
    float3 pos : POSITION;
    float3 normal : NORMAL;
    float4 color : COLOR;
};

struct VertexOut {
    float3 position : POSITION;
    float4 color : COLOR;
    float3 normal : NORMAL;
    float4 lightSpacePos : LIGHT_SPACE_POSITION;
    float4 posW : SV_POSITION;
};

VertexOut BasicVertexShader(VertexIn vIn) {
    VertexOut vOut;

    vOut.position = mul(float4(vIn.pos, 1.0f), world).xyz;
    vOut.posW = mul(float4(vIn.pos, 1.0f), worldViewProj);
    vOut.lightSpacePos = mul(float4(vIn.pos, 1.0f), lightWVP);
    vOut.color = vIn.color;
    vOut.normal = mul(vIn.normal, (float3x3) worldInverse);
    return vOut;
}