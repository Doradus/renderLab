#include "StaticMesh.h"

StaticMesh::StaticMesh() :
	renderData(nullptr)
{}

StaticMesh::~StaticMesh() {
	delete renderData;
	renderData = nullptr;
}

void StaticMesh::SetVertexShader(VertexShader * shader) {
	vertexShader = shader;
}

void StaticMesh::SetPixelShader(PixelShader * shader) {
	pixelShader = shader;
}

void StaticMesh::SetRenderData(RenderData * inData) {
	renderData = inData;
}

VertexShader * StaticMesh::GetVertexShader() const {
	return vertexShader;
}

PixelShader * StaticMesh::GetPixelShader() const {
	return pixelShader;
}

RenderData * StaticMesh::GetRenderData() const {
	return renderData;
}
