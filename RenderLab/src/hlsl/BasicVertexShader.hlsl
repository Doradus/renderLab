struct VertexIn {
    float3 pos : POSITION;
    float3 normal : NORMAL;
    float4 color : COLOR;
};

struct VertexOut {
    float4 color : COLOR;
    float3 normal : NORMAL;
    float4 pos : SV_POSITION;
};

cbuffer ObjectProperties : register(b0) {
    float4x4 worldViewProj;
    float4x4 worldInverse;
};

VertexOut BasicVertexShader(VertexIn vIn) {
    VertexOut vOut;

    vOut.pos = mul(float4(vIn.pos, 1.0f), worldViewProj);
    vOut.color = vIn.color;
    vOut.normal = mul(vIn.normal, (float3x3) worldInverse);
    return vOut;
}