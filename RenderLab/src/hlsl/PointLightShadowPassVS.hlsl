struct VertexIn
{
    float3 pos : POSITION;
    float3 normal : NORMAL;
    float4 color : COLOR;
};

struct VertexOut {
    float4 pos : POSITION;
    float3 normal : NORMAL;
    float4 color : COLOR;
};

cbuffer PerObject : register(b2) {
    float4x4 world;
}

VertexOut PointLightShadowPassVS(VertexIn vIn) {
    VertexOut vOut;
    vOut.pos = mul(float4(vIn.pos, 1.0f), world);
    vOut.normal = vIn.normal;
    vOut.color = vIn.color;
    return vOut;
}