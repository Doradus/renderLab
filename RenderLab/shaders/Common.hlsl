#ifndef __COMMON__
#define __COMMON__

static const float PI = 3.1415926535897932f;

float4 SampleCubeMap(TextureCube textureToSample, SamplerState samplerToUse, float3 v) {
    return textureToSample.Sample(samplerToUse, v);
}


#endif