#include <CommonVS.hlsl>

struct VertexIn {
    float3 pos : POSITION;
    float3 normal : NORMAL;
    float4 color : COLOR;
};

struct VertexOut {
    float3 position : POSITION;
    float4 color : COLOR;
    float3 normal : NORMAL;
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

    
    for (int i = 0; i < 2; i++) {
        vOut.lightSpacePos[i] = mul(worldPosition, lightViewProjection[i]);
    }
  
    vOut.color = vIn.color;
    vOut.normal = mul(vIn.normal, (float3x3) worldInverse);
    return vOut;
}