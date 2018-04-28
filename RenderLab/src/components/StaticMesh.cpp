#include "StaticMesh.h"

StaticMesh::StaticMesh() :
vertexBuffer (nullptr),
indexBuffer(nullptr)
{}

StaticMesh::~StaticMesh() {

}

void StaticMesh::SetVertexBuffer(VertexBuffer* buffer) {
	vertexBuffer = buffer;
}

void StaticMesh::SetIndexBuffer(IndexBuffer* buffer) {
	indexBuffer = buffer;
}

void StaticMesh::SetVertexShader(VertexShader * shader) {
	vertexShader = shader;
}

void StaticMesh::SetPixelShader(PixelShader * shader) {
	pixelShader = shader;
}

VertexBuffer* StaticMesh::GetVertexBuffer() const {
	return vertexBuffer;
}

IndexBuffer* StaticMesh::GetIndexBuffer() const {
	return indexBuffer;
}

VertexShader * StaticMesh::GetVertexShader() const {
	return vertexShader;
}

PixelShader * StaticMesh::GetPixelShader() const {
	return pixelShader;
}
