#include <CommonVS.hlsl>
#include <VertexStructure.hlsl>

struct VertexOut {
    float3 position : POSITION;
    float2 uv : UV;
    float3 normal : NORMAL;
    float3 tangent : TANGENT;
    float4 lightSpacePos [2] : LIGHT_SPACE_POSITION;
    float4 posW : SV_POSITION;
};

VertexOut BasicVertexShader(VertexIn vIn) {
    VertexOut vOut;

    float4 worldPosition = mul(float4(vIn.pos, 1.0f), world);
    vOut.position = worldPosition.xyz;
    vOut.posW = mul(float4(vIn.pos, 1.0f), worldViewProj);

   // float4x4 lightWVP = mul(mul(world, lightView), lightProjection);
   //vOut.lightSpacePos[0] = mul(worldPosition, lightViewProjection[0]);

    [unroll]
    for (int i = 0; i < 2; i++) {
        vOut.lightSpacePos[i] = mul(worldPosition, lightViewProjection[i]);
    }
  
    vOut.uv = vIn.uv;
    vOut.normal = mul(vIn.normal, (float3x3) worldInverse);
    vOut.tangent = mul(vIn.tangent, (float3x3) world);
    return vOut;
}