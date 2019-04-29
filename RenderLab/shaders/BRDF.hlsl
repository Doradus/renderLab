#include "Common.hlsl"

//lambertian BRDF

float3 LambertianDiffuse(float3 color) {
    return color * 1 / PI;
}

/** Normal Distribution (NDF) **/

// Trowbridge Reitz distribution”
float GGXDistribution(float roughness, float NdotH) {
    float a2 = roughness * roughness;
    float d = (NdotH * NdotH) * (a2 - 1.0f) + 1.0f;

    return a2 / (PI * d * d);
}

/** Fresnel **/

//Schlick approximation
float3 SchlickFresnel(float3 specular, float VdotH) {
    return specular + (1.0f - specular) * pow((1.0f - VdotH), 5.0f);
}

/** masking-shadowing function **/

//Smith for ggx
float SmithForGGXMaskingAndShadowing(float roughness, float NdotV, float NdotL) {
    float a2 = roughness * roughness;

    //float d1 = NdotV * sqrt(a2 + NdotL * (NdotL - a2 * NdotL));
    //float d2 = NdotL * sqrt(a2 + NdotV * (NdotV - a2 * NdotV));

    float d1 = NdotL * sqrt((-NdotV * a2 + NdotV) * NdotV + a2);
    float d2 = NdotV * sqrt((-NdotL * a2 + NdotL) * NdotL + a2);

    return 0.5f / (d1 + d2);
}

// Disney diffuse model from "Moving Frostbite to Physically Based Rendering 3.0" presentation 
float DisneyDiffuseModel(float linearRoughness, float NdotV, float NdotL, float LdotH) {
    float energyBias = lerp(0, 0.5, linearRoughness);
    float energyFactor = lerp(1.0, 1.0 / 1.51, linearRoughness);

    float fd90 = energyBias + 2.0 * linearRoughness * LdotH * LdotH;

    return energyFactor * (1 + (fd90 - 1) * pow((1 - NdotL), 5)) * (1 + (fd90 - 1) * pow((1 - NdotV), 5));
}
