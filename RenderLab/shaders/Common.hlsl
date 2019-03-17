#ifndef __COMMON__
#define __COMMON__

static const float PI = 3.1415926535897932f;

float4 SampleCubeMap(TextureCube textureToSample, SamplerState samplerToUse, float3 uv) {
    return textureToSample.Sample(samplerToUse, uv);
}

float4 SampleTexture2d(Texture2D textureToSample, SamplerState samplerToUse, float2 uv) {
    return textureToSample.Sample(samplerToUse, uv);
}

#endif