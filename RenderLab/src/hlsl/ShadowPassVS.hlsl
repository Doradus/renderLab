#include <VertexStructure.hlsl>
struct VertexOut {
    float4 posW : SV_POSITION;
};

cbuffer PerObject : register(b1) {
    float4x4 lightWVP;
}

VertexOut ShadowPassVS(VertexIn vIn) {
    VertexOut vOut;
    vOut.posW = mul(float4(vIn.pos, 1.0f), lightWVP);
    return vOut;
}

