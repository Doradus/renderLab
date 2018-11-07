cbuffer ObjectProperties : register(b0)
{
    float4x4 worldViewProj;
    float4x4 worldInverse;
    float4x4 world;
   // float4x4 lightWVP;
};

cbuffer LightData : register(b3)
{
    float4x4 lightViewProjection [8];
    //float4x4 lightProjection;
};