#include <VertexStructure.hlsl>

struct VertexOut {
    float4 pos : POSITION;
    float3 normal : NORMAL;
    float2 uv : UV;
};

cbuffer PerObject : register(b2) {
    float4x4 world;
}

VertexOut Main(VertexIn vIn) {
    VertexOut vOut;
    vOut.pos = mul(float4(vIn.pos, 1.0f), world);
    vOut.normal = vIn.normal;
    vOut.uv = vIn.uv;
    return vOut;
}