#pragma once
#include <vector>
#include "Math.h"

using namespace DirectX;

struct Vertex {
	XMFLOAT3 pos;
	XMFLOAT4 color;
};

struct MeshData {
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
};