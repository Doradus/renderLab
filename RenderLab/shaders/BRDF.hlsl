#include "Common.hlsl"

//lambertian BRDF

float3 LambertianDiffuse(float3 color) {
    return color * 1 / PI;
}

/** Normal Distribution (NDF) **/

// Trowbridge Reitz distribution”
float GGXDistribution(float roughness, float NoH) {
    float a2 = roughness * roughness;
    float d = (NoH * NoH) * (a2 - 1.0f) + 1.0f;

    return a2 / (PI * d * d);
}

/** Fresnel **/

//Schlick approximation
float3 SchlickFresnel(float3 specular, float VoH) {
    return specular + (1.0f - specular) * pow((1.0f - saturate(VoH)), 5.0f);
}

/** masking-shadowing function **/

//Smith for ggx
float SmithForGGXMaskingAndShadowing(float roughness, float NoV, float NoL) {
    float a2 = roughness * roughness;

    float d1 = NoV * sqrt(a2 + NoL * (NoL - a2 * NoL));
    float d2 = NoL * sqrt(a2 + NoV * (NoV - a2 * NoV));

    return 0.5f / (d1 + d2);
}
