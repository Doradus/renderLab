#pragma once
#include <vector>
#include "MathUtils.h"

using namespace DirectX;


struct Vertex {
	Vertex() {}
	Vertex(const XMFLOAT3 &inPos, const XMFLOAT3 &inNormal) :
		position(inPos),
		normal(inNormal) {}

	Vertex(float inPosX, float inPosY, float inPosZ,
		float inNormX, float inNormY, float inNormZ) :
		position(inPosX, inPosY, inPosZ),
		normal(inNormX, inNormY, inNormZ) {}

	XMFLOAT3 position;
	XMFLOAT3 normal;
};

struct MeshData {
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
};

struct ObjectProperties {
	XMFLOAT4X4 wvp;
	XMFLOAT4X4 worldInverse;
	XMFLOAT4X4 world;
	//XMFLOAT4X4 lightWVP;
};

struct DirectionalLightResource {
	DirectionalLightResource() {};
	XMFLOAT3 color;
	int	enabled; 
	XMFLOAT3 direction;
	float brightness;
};

struct PointLightResources {
	PointLightResources() {};
	XMFLOAT3 color;
	bool enabled;
	XMFLOAT3 position;
	float range;
	XMFLOAT3 attenuation;
	float brightness;
};

struct LightPropertiesResource {
	LightPropertiesResource() {};
	XMFLOAT3 color;
	bool enabled;
	/* ------------------------- */
	XMFLOAT3 position;
	float range;
	/* ------------------------- */
	XMFLOAT3 attenuation;
	float brightness;
	/* ------------------------- */
	XMFLOAT3 direction;
	float type;
	/* ------------------------- */
	float coneAngle;
	float penumbraAngle;
	int useShadow;
	float shadowMapId;
};

struct PixelShaderPerFrameResource {
	PixelShaderPerFrameResource() {};
	LightPropertiesResource lightResources[8];
	XMFLOAT3 cameraPosition;
	unsigned int activeLights;
};

struct MaterialResource {
	MaterialResource() {};
	XMFLOAT3 albedo;
	float specularPower;
	XMFLOAT3 specularColor;
	float padding;
};

struct OminDirectionalShadowPassVSResources {
	XMFLOAT4X4 worldMatrix;
};

struct OmniDirectionalShadowPassGSResources {
	XMFLOAT4X4 lightVPMatrix[6];
};

struct LightSpaceTransformBuffer {
	XMFLOAT4X4 lightViewProjection [8];
	//XMFLOAT4X4 lightProjection;
};

struct VertexShaderShadowResources {
	VertexShaderShadowResources() {};
	XMFLOAT4X4 lightWorldViewProj;
};

enum VertexConstBuffers {
	WorldViewPorjectionConstBuffer,
	ShadowConstantBuffer,
	NumVertexConstantBuffers
};

enum PixelConstBuffers {
	DirectionalLightsConstBuffer,
	MaterialConstBuffer,
	NumPixelConstantBuffers
};