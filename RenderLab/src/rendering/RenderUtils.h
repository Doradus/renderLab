#pragma once
#include <vector>
#include "MathUtils.h"

using namespace DirectX;

struct Vertex {
	Vertex() {}
	Vertex(const XMFLOAT3 &inPos, const XMFLOAT4 &inColor) :
		pos(inPos),
		color(inColor) {}

	Vertex(float inPosX, float inPosY, float inPosZ, float inColorR, float inColorG, float inColorB, float inColorA) :
		pos(inPosX, inPosY, inPosZ),
		color(inColorR, inColorG, inColorB, inColorA) {}

	XMFLOAT3 pos;
	XMFLOAT4 color;
};

struct MeshData {
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
};