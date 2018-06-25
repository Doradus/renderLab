struct VertexIn {
    float3 pos : POSITION;
    float3 normal : NORMAL;
    float4 color : COLOR;
};

struct VertexOut {
    float4 posW : SV_POSITION;
};

cbuffer PerObject : register(b0) {
    float4x4 lightWVP;
}

VertexOut ShadowPassVS(VertexIn vIn) {
    VertexOut vOut;
    vOut.posW = mul(float4(vIn.pos, 1.0f), lightWVP);
    return vOut;
}

