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
	float padding1;
	float padding2;
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

enum VertexConstBuffers {
	WorldViewPorjectionConstBuffer,
	NumVertexConstantBuffers
};

enum PixelConstBuffers {
	DirectionalLightsConstBuffer,
	MaterialConstBuffer,
	NumPixelConstantBuffers
};