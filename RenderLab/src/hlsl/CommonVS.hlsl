cbuffer ObjectProperties : register(b0)
{
    float4x4 worldViewProj;
    float4x4 worldInverse;
    float4x4 world;
};