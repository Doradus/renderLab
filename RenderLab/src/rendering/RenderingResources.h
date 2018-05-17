#pragma once
#include <vector>
#include "MathUtils.h"

using namespace DirectX;

struct Vertex {
	Vertex() {}
	Vertex(const XMFLOAT3 &inPos, const XMFLOAT3 &inNormal, const XMFLOAT4 &inColor) :
		position(inPos),
		normal(inNormal),
		color(inColor) {}

	Vertex(float inPosX, float inPosY, float inPosZ,
		float inNormX, float inNormY, float inNormZ,
		float inColorR, float inColorG, float inColorB, float inColorA) :
		position(inPosX, inPosY, inPosZ),
		normal(inNormX, inNormY, inNormZ),
		color(inColorR, inColorG, inColorB, inColorA) {}

	XMFLOAT3 position;
	XMFLOAT3 normal;
	XMFLOAT4 color;
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
	float padding1;
	XMFLOAT3 direction;
	float padding2;
};

enum ConstBuffers {
	WorldViewPorjection,
	DirectionalLights,
	NumConstantBuffers
};