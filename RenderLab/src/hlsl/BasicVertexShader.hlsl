cbuffer perObject {
    float4x4 worldViewProj;
};

struct VertexIn {
    float3 pos : POSITION;
    float4 color : COLOR;
};

struct VertexOut {
    float4 pos : SV_POSITION;
    float4 color : COLOR;
};

VertexOut BasicVertexShader(VertexIn vIn) {
    VertexOut vOut;

    vOut.pos = mul(float4(vIn.pos, 1.0f), worldViewProj);
    vOut.color = vIn.color;

    return vOut;
}