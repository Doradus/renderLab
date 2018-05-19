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
};

struct DirectionalLightResource {
	DirectionalLightResource() {};

	XMFLOAT3 color;
	int	enabled; 
	XMFLOAT3 position;
	float brightness;
	XMFLOAT3 direction;
	float padding;
};

struct MaterialResource {
	MaterialResource() {};

	XMFLOAT3 albedo;
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