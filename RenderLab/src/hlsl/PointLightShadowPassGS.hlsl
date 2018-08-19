struct VertexIn
{
    float4 pos : POSITION;
    float3 normal : NORMAL;
    float4 color : COLOR;
};

struct GeoOut
{
    float4 color : COLOR;
    float3 normal : NORMAL;
    uint renderTargetIndex : SV_RenderTargetArrayIndex;
    float4 posW : SV_POSITION;
};

cbuffer PerFrame : register(b0)
{
    float4x4 lightVP [6];
}

[maxvertexcount(18)]
void PointLightShadowPassGS(triangle VertexIn gIn[3], inout TriangleStream<GeoOut> gOut) {

    [unroll]
    for (int face = 0; face < 6; face++) {
        GeoOut output;
        output.renderTargetIndex = face;

        [unroll]
        for (int i = 0; i < 3; i++) {
            output.posW = mul(gIn[i].pos, lightVP[face]);
            output.normal = gIn[i].normal;
            output.color = gIn[i].color;

            gOut.Append(output);
        }

        gOut.RestartStrip();
    }
}